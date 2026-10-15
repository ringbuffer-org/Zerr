/**
 * @file    zerr_features.hpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   Header for the ZerrFeatures class used in the zerr.features~ Max/MSP external
 * @date    2025-05-01
 *
 * @copyright  Copyright (c) 2023-2026 Zeyu Yang
 * @license    MIT license
 */
#pragma once

#include <algorithm>
#include <atomic>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

#include "featurebank.h"
#include "types.h"
#include "zerr_block_lock.hpp"

/**
 * @class ZerrFeatures
 * @brief Wrapper between Max/MSP's audio system and the core feature extraction
 *
 * Construction only records the feature names. The core zerr::FeatureBank is built by the
 * first prepare(), at creation with Max's global sample rate and vector size (so a
 * misspelled feature name fails creation); every dsp64 then calls prepare() again with the
 * shape of the signal chain the object actually lives in, and the core re-sizes itself in
 * place (see zerr::SystemConfigs). The old wrapper sized its buffers once at creation and
 * overflowed the heap when the vector size grew.
 */
class ZerrFeatures {
  public:
    /**
     * @brief Creates a new ZerrFeatures instance
     * @param names Short names of the features to extract, one signal outlet each
     */
    explicit ZerrFeatures(zerr::FeatureNames names) : featureNames{std::move(names)} {}

    // Neither copyable nor movable: holds an atomic and is owned by a raw pointer in the
    // Max object struct, so neither operation is needed.
    ZerrFeatures(const ZerrFeatures&)            = delete;
    ZerrFeatures& operator=(const ZerrFeatures&) = delete;
    ZerrFeatures(ZerrFeatures&&)                 = delete;
    ZerrFeatures& operator=(ZerrFeatures&&)      = delete;

    /**
     * @brief Adopts the sample rate and block size of the signal chain, as dsp64 reports them
     * @param cfg   Sample rate and block size
     * @param error Receives the core's message when a feature name is unknown or the shape
     *              is out of range (block size 1..AUDIO_BUFFER_SIZE)
     * @return true if the object is ready to process
     *
     * Main thread only. The first call builds the core FeatureBank (so a misspelled feature
     * name fails creation); later calls hand the shape to FeatureBank::prepare(), which is
     * idempotent, so toggling DSP costs nothing.
     */
    bool prepare(const zerr::SystemConfigs& cfg, std::string* error = nullptr)
    {
        // Max may compile the new DSP chain while the old one still calls perform(); the
        // lock keeps the resize below off a running block (see zerr_block_lock.hpp). The
        // ready flag covers the states the lock does not: never prepared, prepare() failed.
        zerr_max::BlockGuard guard(lock);
        try {
            if (!bank) {
                auto next = std::make_unique<zerr::FeatureBank>();
                next->initialize(featureNames, cfg);
                bank = std::move(next);
            }
            else if (!bank->prepare(cfg)) {
                if (error) {
                    *error = "block size must be 1.." + std::to_string(zerr::AUDIO_BUFFER_SIZE) +
                             " and sample rate non-zero (got block " +
                             std::to_string(cfg.block_size) + ", rate " +
                             std::to_string(cfg.sample_rate) + ")";
                }
                return false;
            }
        }
        catch (const std::exception& e) {
            if (error) {
                *error = e.what();
            }
            return false;
        }

        inputBuffer.assign(cfg.block_size, 0.0);
        systemConfigs = cfg;
        ready.store(true, std::memory_order_release);
        return true;
    }

    /**
     * @brief Main DSP callback function that processes audio buffers
     * @param ins Array of pointers to input audio buffers
     * @param numins Number of input channels
     * @param outs Array of pointers to output audio buffers
     * @param numouts Number of output channels
     * @param sampleframes Number of samples to process
     *
     * noexcept by contract: this runs on the audio thread, inside Max's C call stack, where
     * an escaping exception would terminate the host. Anything unexpected zeroes the output.
     */
    void perform(double** ins, long numins, double** outs, long numouts, long sampleframes) noexcept
    {
        const long numOut = static_cast<long>(featureNames.size());

        zerr_max::TryBlockGuard guard(lock); // silence for the one block prepare() is resizing
        if (!guard.ok || !ready.load(std::memory_order_acquire) || !ins || !outs ||
            numins < inputCount || numouts < numOut || !ins[0]) {
            silence(outs, numouts, sampleframes);
            return;
        }

        // The core sizes every feature's output from block_size; never read or write past it.
        const long block = static_cast<long>(systemConfigs.block_size);
        const long count = std::min<long>(sampleframes, block);

        std::copy_n(ins[0], count, inputBuffer.begin());
        if (count < block) {
            std::fill(inputBuffer.begin() + count, inputBuffer.end(), 0.0);
        }

        try {
            outputBuffer = bank->perform(inputBuffer);
        }
        catch (...) {
            silence(outs, numouts, sampleframes);
            return;
        }

        if (static_cast<long>(outputBuffer.size()) < numOut) {
            silence(outs, numouts, sampleframes);
            return;
        }

        for (long i = 0; i < numOut; ++i) {
            if (!outs[i]) {
                continue;
            }
            const long have = std::min<long>(count, static_cast<long>(outputBuffer[i].size()));
            std::copy_n(outputBuffer[i].begin(), have, outs[i]);
            if (have < sampleframes) {
                std::memset(outs[i] + have, 0, (sampleframes - have) * sizeof(double));
            }
        }
    }

    /**
     * @brief Gets the number of output channels (one per feature)
     */
    [[nodiscard]] int getOutputCount() const noexcept
    {
        return static_cast<int>(featureNames.size());
    }

    /**
     * @brief Gets the number of input channels (fixed at 1)
     */
    [[nodiscard]] constexpr int getInputCount() const noexcept { return inputCount; }

    /**
     * @brief The feature names in outlet order
     */
    [[nodiscard]] const zerr::FeatureNames& getFeatureNames() const noexcept
    {
        return featureNames;
    }

    /**
     * @brief The sample rate and block size the bank was last prepared for (0 if never)
     */
    [[nodiscard]] const zerr::SystemConfigs& getSystemConfigs() const noexcept
    {
        return systemConfigs;
    }

    ~ZerrFeatures() = default;

  private:
    /**
     * @brief Zeroes every output buffer; the fallback whenever processing cannot proceed
     */
    static void silence(double** outs, long numouts, long sampleframes) noexcept
    {
        if (!outs) {
            return;
        }
        for (long i = 0; i < numouts; ++i) {
            if (outs[i]) {
                std::memset(outs[i], 0, sampleframes * sizeof(double));
            }
        }
    }

    static constexpr int inputCount = 1; /**< One mono signal inlet */

    zerr::FeatureNames featureNames;         /**< Features to extract, in outlet order */
    zerr::SystemConfigs systemConfigs{0, 0}; /**< Shape the bank was prepared for */

    std::atomic<bool> ready{false}; /**< True once the bank is built and buffers sized */
    zerr_max::BlockLock lock;       /**< Keeps prepare() off a block perform() is processing */

    zerr::Samples inputBuffer;       /**< One block of input, sized to block_size */
    zerr::FeaturesVals outputBuffer; /**< The core's per-feature output, one block each */

    std::unique_ptr<zerr::FeatureBank> bank; /**< Core feature extraction */
};
