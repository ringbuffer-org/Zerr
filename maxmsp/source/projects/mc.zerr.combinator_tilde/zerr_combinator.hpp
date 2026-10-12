/**
 * @file    zerr.combinator.hpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   Header for the ZerrCombinator class used in mc.zerr.combinator~ Max/MSP External
 * @date    2025-05-03
 *
 * @copyright  Copyright (c) 2023-2025
 * @license    MIT license
 */
#pragma once

#include <algorithm>
#include <atomic>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "./envelopecombinator.h"
#include "types.h"

/**
 * @class ZerrCombinator
 * @brief Manages the combination of multiple envelope signals across channel groups
 *
 * This class handles the processing of multiple multichannel envelope signals,
 * combining them according to the specified mode. It provides a clean interface
 * between Max/MSP's audio system and the underlying envelope combination algorithms.
 *
 * Construction is split in two because Max hands over the object's shape in stages:
 * the number of envelope sets is known when the object is created, but the number of
 * envelopes per set (the multichannel channel count), the sample rate and the block
 * size only arrive in dsp64. The core zerr::EnvelopeCombinator therefore cannot be
 * built by the constructor -- prepare() builds it.
 */
class ZerrCombinator {
  public:
    /**
     * @brief Creates a new ZerrCombinator instance
     * @param inputCount Number of envelope sets to combine (one multichannel inlet each)
     * @param mode       Combination mode: "add", "root" or "max"
     * @throws std::invalid_argument if inputCount < 1 or mode is not a known mode
     */
    ZerrCombinator(int inputCount, const std::string& mode)
        : inputCount{inputCount}, combMode{zerr::parseCombMode(mode)}
    {
        if (inputCount < 1) {
            throw std::invalid_argument("ZerrCombinator: inputCount must be at least 1");
        }
    }

    // Neither copyable nor movable: holds an atomic and is owned by a raw pointer in the
    // Max object struct, so neither operation is needed.
    ZerrCombinator(const ZerrCombinator&)            = delete;
    ZerrCombinator& operator=(const ZerrCombinator&) = delete;
    ZerrCombinator(ZerrCombinator&&)                 = delete;
    ZerrCombinator& operator=(ZerrCombinator&&)      = delete;

    /**
     * @brief Builds (or rebuilds) the core module for a given signal shape
     * @param cfg Sample rate and block size, as reported by dsp64
     * @param numChannel Number of envelopes per set, i.e. the inlets' channel count
     * @return true if the combinator is ready to process
     *
     * Called from dsp64 on the main thread, where allocation is safe. Idempotent: returns
     * immediately when the shape has not changed, so toggling DSP does not reallocate.
     */
    bool prepare(const zerr::SystemConfigs& cfg, int numChannel)
    {
        if (numChannel < 1 || cfg.block_size == 0) {
            return false;
        }

        if (ready.load(std::memory_order_acquire) && numChannel == this->numChannel &&
            cfg.block_size == systemConfigs.block_size &&
            cfg.sample_rate == systemConfigs.sample_rate) {
            return true;
        }

        auto next = std::make_unique<zerr::EnvelopeCombinator>(inputCount, numChannel, cfg,
                                                               zerr::toString(combMode));
        if (!next->initialize()) {
            return false;
        }

        zerr::Blocks nextIn(next->numInlet, zerr::Samples(cfg.block_size, 0.0));

        // MSP rebuilds the DSP chain around dsp64, so perform() is not running for this
        // object while we swap. The flag covers the states MSP does not: never prepared,
        // and prepare() having failed.
        ready.store(false, std::memory_order_release);
        combinator.swap(next);
        inputBuffer.swap(nextIn);
        systemConfigs    = cfg;
        this->numChannel = numChannel;
        ready.store(true, std::memory_order_release);

        return true; // the previous combinator dies with `next` here
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
        const long numIn  = static_cast<long>(inputBuffer.size());
        const long numOut = numChannel;

        if (!ready.load(std::memory_order_acquire) || !ins || !outs || numins < numIn ||
            numouts < numOut) {
            silence(outs, numouts, sampleframes);
            return;
        }

        // The core sizes its buffers from block_size; never read or write past them.
        const long count =
            std::min<long>(sampleframes, static_cast<long>(systemConfigs.block_size));

        for (long i = 0; i < numIn; ++i) {
            if (!ins[i]) {
                silence(outs, numouts, sampleframes);
                return;
            }
            std::copy_n(ins[i], count, inputBuffer[i].begin());
        }

        // Read the core's buffer directly rather than copying it into one of our own.
        const zerr::Blocks& combined = combinator->perform(inputBuffer);
        if (static_cast<long>(combined.size()) < numOut) {
            silence(outs, numouts, sampleframes);
            return;
        }

        for (long i = 0; i < numOut; ++i) {
            if (!outs[i]) {
                continue;
            }
            std::copy_n(combined[i].begin(), count, outs[i]);
            if (count < sampleframes) {
                std::memset(outs[i] + count, 0, (sampleframes - count) * sizeof(double));
            }
        }
    }

    /**
     * @brief Changes the combination mode, with or without DSP running
     * @param mode Mode name: "add", "root" or "max"
     * @return false, leaving the mode unchanged, if the name is not a known mode
     *
     * Control thread only. Nothing is rebuilt: the core swaps a lock-free atomic, and the
     * mode is remembered here too so that a change made before DSP ever started is applied
     * by the next prepare().
     */
    bool setMode(const std::string& mode) noexcept
    {
        zerr::CombMode parsed;
        if (!zerr::tryParseCombMode(mode, parsed)) {
            return false;
        }
        combMode = parsed;
        if (combinator) {
            combinator->set_mode(parsed);
        }
        return true;
    }

    /**
     * @brief Gets the combination mode currently in use
     */
    [[nodiscard]] const char* getMode() const noexcept { return zerr::toString(combMode); }

    /**
     * @brief Gets the number of envelope sets being combined
     */
    [[nodiscard]] int getInputCount() const noexcept { return inputCount; }

    /**
     * @brief Gets the number of envelopes per set, 0 until prepare() has succeeded
     */
    [[nodiscard]] int getChannelCount() const noexcept { return numChannel; }

    ~ZerrCombinator() = default;

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

    int inputCount;          /**< Number of envelope sets, one multichannel inlet each */
    int numChannel{0};       /**< Envelopes per set; known only once prepare() has run */
    zerr::CombMode combMode; /**< Combination mode; control thread only, mirrored into the core */

    zerr::SystemConfigs systemConfigs{0, 0}; /**< System configuration settings */

    std::atomic<bool> ready{false}; /**< True once the core module is built and buffers sized */

    zerr::Blocks inputBuffer; /**< Buffer for storing incoming audio samples */

    std::unique_ptr<zerr::EnvelopeCombinator> combinator; /**< Core combination algorithms */
};
