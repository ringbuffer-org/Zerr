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

/**
 * @class ZerrFeatures
 * @brief Wrapper between Max/MSP's audio system and the core feature extraction
 *
 * Construction only records the feature names. The core zerr::FeatureBank is built by
 * prepare(), which runs once at creation with Max's global sample rate and vector size
 * (so a misspelled feature name fails creation) and again from dsp64 with the values of
 * the signal chain the object actually lives in. Those differ inside a poly~ running at
 * another rate or vector size, and the old wrapper, which sized its buffers once at
 * creation, overflowed the heap when the vector size grew.
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
     * @brief Builds (or rebuilds) the core feature bank for a given signal shape
     * @param cfg   Sample rate and block size, as reported by dsp64
     * @param error Receives the core's message when a feature name is unknown
     * @return true if the object is ready to process
     *
     * Main thread only. Idempotent: returns at once when the shape has not changed, so
     * toggling DSP does not reallocate. A new FeatureBank is built every time because
     * FeatureBank::initialize() appends to its extractor list rather than replacing it.
     * The 2048-sample analysis history restarts from silence on a reshape.
     */
    bool prepare(const zerr::SystemConfigs& cfg, std::string* error = nullptr)
    {
        if (cfg.block_size == 0 || cfg.sample_rate == 0) {
            if (error) {
                *error = "block size and sample rate must be positive";
            }
            return false;
        }

        if (ready.load(std::memory_order_acquire) && cfg.block_size == systemConfigs.block_size &&
            cfg.sample_rate == systemConfigs.sample_rate) {
            return true;
        }

        auto next = std::make_unique<zerr::FeatureBank>();
        try {
            next->initialize(featureNames, cfg);
        }
        catch (const std::exception& e) {
            if (error) {
                *error = e.what();
            }
            return false;
        }

        zerr::Samples nextIn(cfg.block_size, 0.0);

        // MSP rebuilds the DSP chain around dsp64, so perform() is not running for this
        // object while we swap. The flag covers the states MSP does not: never prepared,
        // and prepare() having failed.
        ready.store(false, std::memory_order_release);
        bank.swap(next);
        inputBuffer.swap(nextIn);
        systemConfigs = cfg;
        ready.store(true, std::memory_order_release);

        return true; // the previous bank dies with `next` here
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

        if (!ready.load(std::memory_order_acquire) || !ins || !outs || numins < inputCount ||
            numouts < numOut || !ins[0]) {
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

    zerr::Samples inputBuffer;       /**< One block of input, sized to block_size */
    zerr::FeaturesVals outputBuffer; /**< The core's per-feature output, one block each */

    std::unique_ptr<zerr::FeatureBank> bank; /**< Core feature extraction */
};
