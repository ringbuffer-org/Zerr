/**
 * @file    zerr_envelopes.hpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   Header for the ZerrEnvelopes class used in the zerr.envelopes~ Max/MSP external
 * @date    2025-05-03
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
#include <vector>

#include "envelopegenerator.h"
#include "logger.h"
#include "types.h"
#include "zerr_block_lock.hpp"

/**
 * @class ZerrEnvelopes
 * @brief Wrapper between Max/MSP's audio system and the core envelope generation
 *
 * The core zerr::EnvelopeGenerator is built once, in the constructor, because it owns
 * the speaker state the user shapes with messages (active set, trajectory, topology,
 * current speaker). A change of sample rate or block size reported by dsp64 is handed
 * to it through prepare(), which resizes its blocks and keeps that state; the old
 * wrapper froze the creation-time vector size and overflowed the heap when it grew.
 */
class ZerrEnvelopes {
  public:
    /**
     * @brief Creates a new ZerrEnvelopes instance
     * @param sampleRate    Sample rate the generator is first sized for
     * @param blockSize     Block size the generator is first sized for
     * @param selectionMode "trajectory" or "trigger"
     * @param layoutName    The speaker layout file path, or a label when layoutYaml is given
     * @param layoutYaml    YAML text of the layout; empty to read the file at layoutName
     * @param printer       Where the core's log lines go (installed before anything can log)
     * @throws std::invalid_argument if selectionMode is not a known mode
     */
    ZerrEnvelopes(float sampleRate, int blockSize, const std::string& selectionMode,
                  std::string layoutName, const std::string& layoutYaml,
                  zerr::Logger::PrintStrategy printer)
        : systemConfigs{static_cast<size_t>(sampleRate), static_cast<size_t>(blockSize)},
          selectionMode{selectionMode}, layoutName{std::move(layoutName)},
          generator{std::make_unique<zerr::EnvelopeGenerator>(systemConfigs, this->layoutName,
                                                              zerr::parseGenMode(selectionMode))}
    {
        generator->setPrinter(std::move(printer));
        // INFO so that `print` (printParameters logs at INFO) reaches the Console.
        generator->setLogLevel(zerr::LogLevel::INFO);
        if (!layoutYaml.empty()) {
            generator->setSpeakerConfigText(layoutYaml);
        }
    }

    // Neither copyable nor movable: holds an atomic and is owned by a raw pointer in the
    // Max object struct, so neither operation is needed.
    ZerrEnvelopes(const ZerrEnvelopes&)            = delete;
    ZerrEnvelopes& operator=(const ZerrEnvelopes&) = delete;
    ZerrEnvelopes(ZerrEnvelopes&&)                 = delete;
    ZerrEnvelopes& operator=(ZerrEnvelopes&&)      = delete;

    /**
     * @brief Loads the speaker layout and sizes the buffers for the creation-time shape
     * @param error Receives a message when loading fails
     * @return true if the object is ready to process
     *
     * Main thread only, once. The core reports load failures through its logger (already
     * routed to the Console) and by returning false; a malformed document may also throw
     * from yaml-cpp, which is caught here so that a bad file never reaches Max's C stack.
     */
    bool initialize(std::string* error = nullptr)
    {
        bool ok = false;
        try {
            ok = generator->initialize();
        }
        catch (const std::exception& e) {
            if (error) {
                *error = "cannot read speaker layout " + layoutName + ": " + e.what();
            }
            return false;
        }
        if (!ok) {
            if (error) {
                *error = "cannot load speaker layout " + layoutName;
            }
            return false;
        }

        outputCount = generator->getNumSpeakers();
        if (outputCount < 1) {
            if (error) {
                *error = "speaker layout " + layoutName + " defines no speakers under 'standard:'";
            }
            return false;
        }

        inputBuffer.assign(inputCount, zerr::Samples(systemConfigs.block_size, 0.0));
        initialized = true;
        ready.store(true, std::memory_order_release);
        return true;
    }

    /**
     * @brief Adopts the sample rate and block size of the signal chain, as dsp64 reports them
     * @return true if the object is ready to process
     *
     * Main thread only. The core's prepare() is idempotent, so toggling DSP costs nothing.
     * The speaker state and the trigger interval survive; only the blocks are resized.
     */
    bool prepare(const zerr::SystemConfigs& cfg)
    {
        if (!initialized) {
            return false;
        }

        // Max may compile the new DSP chain while the old one still calls perform(); the
        // lock keeps the resize off a running block (see zerr_block_lock.hpp).
        zerr_max::BlockGuard guard(lock);
        if (!generator->prepare(cfg)) {
            return false;
        }
        inputBuffer.assign(inputCount, zerr::Samples(cfg.block_size, 0.0));
        systemConfigs = cfg;
        ready.store(true, std::memory_order_release);
        return true;
    }

    /**
     * @brief Main DSP callback function that processes audio buffers
     *
     * noexcept by contract: this runs on the audio thread, inside Max's C call stack, where
     * an escaping exception would terminate the host. Anything unexpected zeroes the output.
     */
    void perform(double** ins, long numins, double** outs, long numouts, long sampleframes) noexcept
    {
        const long numOut = outputCount;

        zerr_max::TryBlockGuard guard(lock); // silence for the one block prepare() is resizing
        if (!guard.ok || !ready.load(std::memory_order_acquire) || !ins || !outs ||
            numins < inputCount || numouts < numOut) {
            silence(outs, numouts, sampleframes);
            return;
        }

        // The core sizes its blocks from block_size; never read or write past them.
        const long block = static_cast<long>(systemConfigs.block_size);
        const long count = std::min<long>(sampleframes, block);

        for (int i = 0; i < inputCount; ++i) {
            if (i == gainInlet && !gainConnected.load(std::memory_order_relaxed)) {
                // Nothing on the gain inlet: Max hands over zeros, which would mute every
                // envelope. Unity gain instead, so a plain object plays.
                std::fill(inputBuffer[i].begin(), inputBuffer[i].end(), 1.0);
                continue;
            }
            if (!ins[i]) {
                silence(outs, numouts, sampleframes);
                return;
            }
            std::copy_n(ins[i], count, inputBuffer[i].begin());
            if (count < block) {
                std::fill(inputBuffer[i].begin() + count, inputBuffer[i].end(), 0.0);
            }
        }

        try {
            outputBuffer = generator->perform(inputBuffer);
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

    /** @brief Number of output channels: one envelope per speaker in the layout */
    [[nodiscard]] int getOutputCount() const noexcept { return outputCount; }

    /** @brief Number of signal inlets (fixed at 3: control, spread, gain) */
    [[nodiscard]] constexpr int getInputCount() const noexcept { return inputCount; }

    /** @brief The mode name this object was created with */
    [[nodiscard]] const std::string& getMode() const noexcept { return selectionMode; }

    /** @brief The layout file path, or the built-in layout's label */
    [[nodiscard]] const std::string& getLayoutName() const noexcept { return layoutName; }

    /** @brief The sample rate and block size the object is currently sized for */
    [[nodiscard]] const zerr::SystemConfigs& getSystemConfigs() const noexcept
    {
        return systemConfigs;
    }

    ~ZerrEnvelopes() = default;

    // ---- control-thread setters, forwarded to the core ----

    void setActiveSpeakerIndexs(const char* action, const int* idxs, size_t size)
    {
        generator->setActiveSpeakerIndexs(action, zerr::Indexes(idxs, idxs + size));
    }

    void setCurrentSpeaker(int idx) { generator->setCurrentSpeaker(idx); }

    void setTopoMatrix(const char* action, const int* idxs, size_t size)
    {
        generator->setTopoMatrix(action, zerr::Indexes(idxs, idxs + size));
    }

    void setTrajectoryVector(const int* idxs, size_t size)
    {
        generator->setTrajectoryVector(zerr::Indexes(idxs, idxs + size));
    }

    /** @brief Sets the minimum time between two trigger jumps, in milliseconds */
    void setTriggerInterval(float interval) { generator->setTriggerInterval(interval); }

    /** @brief Posts the speaker setup to the Console */
    void printParameters() { generator->printParameters(); }

    /**
     * @brief Whether a patch cord is on the gain inlet, from dsp64's count[]
     *
     * Unconnected, the inlet counts as unity gain. Main thread, at DSP setup; the running
     * perform() picks it up on its next block.
     */
    void setGainConnected(bool connected)
    {
        gainConnected.store(connected, std::memory_order_relaxed);
    }

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

    static constexpr int inputCount = 3;    /**< Signal inlets: control(0), spread(1), gain(2) */
    static constexpr int gainInlet  = 2;    /**< The inlet that reads 1 when nothing is connected */
    std::atomic<bool> gainConnected{false}; /**< A cord is on the gain inlet (dsp64 count[]) */
    int outputCount = 0;                    /**< One envelope per speaker in the layout */

    zerr::SystemConfigs systemConfigs; /**< Shape the generator is currently sized for */
    std::string selectionMode;         /**< "trajectory" or "trigger" */
    std::string layoutName;            /**< File path, or the built-in layout's label */

    bool initialized = false;       /**< initialize() succeeded */
    std::atomic<bool> ready{false}; /**< Blocks are sized for systemConfigs; perform may run */
    zerr_max::BlockLock lock;       /**< Keeps prepare() off a block perform() is processing */

    zerr::Blocks inputBuffer;  /**< One block per inlet, sized to block_size */
    zerr::Blocks outputBuffer; /**< The core's output, one block per speaker */

    std::unique_ptr<zerr::EnvelopeGenerator> generator; /**< Core envelope generation */
};
