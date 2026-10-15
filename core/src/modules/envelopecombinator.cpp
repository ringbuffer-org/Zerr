/**
 * @file envelopecombinator.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Envelope Combinator Class Implementation
 * @date 2024-02-03
 *
 * @copyright Copyright (c) 2023-2024
 */
#include "envelopecombinator.h"

#include <algorithm>
using namespace zerr;

EnvelopeCombinator::EnvelopeCombinator(int numSource, int numChannel, SystemConfigs systemCfgs,
                                       std::string combMode)
{
    this->numSource  = numSource;
    this->numChannel = numChannel;
    this->systemCfgs = systemCfgs;

    // Parse now, report in initialize(). The constructor cannot fail, and wrappers
    // already gate on initialize()'s return value.
    CombMode parsed;
    modeValid = tryParseCombMode(combMode, parsed);
    if (modeValid) {
        this->combMode.store(parsed, std::memory_order_relaxed);
    }
    this->requestedModeName = combMode;

    numInlet  = numSource * numChannel;
    numOutlet = numChannel;

#ifdef TESTMODE
    logger.setLogLevel(LogLevel::INFO);
#endif // TESTMODE
}

bool EnvelopeCombinator::initialize()
{
    if (!modeValid) {
        logger.logError("EnvelopeCombinator::initialize Unknown combination mode: " +
                        requestedModeName);
        return false;
    }

    inputBuffer.assign(numInlet, Samples());
    outputBuffer.assign(numOutlet, Samples());
    const SystemConfigs cfg = systemCfgs;
    systemCfgs              = SystemConfigs{0, 0}; // fresh buffers are never "already prepared"
    if (!prepare(cfg)) {
        logger.logError("EnvelopeCombinator::initialize block size and sample rate must be > 0");
        return false;
    }
    return true;
}

bool EnvelopeCombinator::prepare(SystemConfigs cfg)
{
    if (cfg.block_size == 0 || cfg.sample_rate == 0) {
        return false;
    }
    if (cfg == systemCfgs) {
        return true;
    }

    systemCfgs = cfg;
    for (auto& block : inputBuffer) {
        block.assign(systemCfgs.block_size, 0.0f);
    }
    for (auto& block : outputBuffer) {
        block.assign(systemCfgs.block_size, 0.0f);
    }
    prepared = true;
    return true;
}

bool EnvelopeCombinator::set_mode(const std::string& mode) noexcept
{
    CombMode parsed;
    if (!tryParseCombMode(mode, parsed)) {
        return false;
    }
    set_mode(parsed);
    return true;
}

void EnvelopeCombinator::set_mode(CombMode mode) noexcept
{
    // Relaxed is enough: the enum is the only datum published, and it guards nothing else.
    combMode.store(mode, std::memory_order_relaxed);
}

const Blocks& EnvelopeCombinator::perform(const Blocks& in)
{
    if (!prepared || static_cast<int>(in.size()) < numInlet) {
        for (auto& block : outputBuffer) {
            block.assign(block.size(), 0.0f);
        }
        return outputBuffer;
    }

    // Copy into the prepared blocks, never past them; a short input leaves silence behind.
    // The frame count is the shortest input, so no loop reads past any of them.
    size_t frames = systemCfgs.block_size;
    for (int i = 0; i < numInlet; ++i) {
        Samples& dst       = inputBuffer[i];
        const size_t count = std::min(in[i].size(), dst.size());
        std::copy_n(in[i].begin(), count, dst.begin());
        std::fill(dst.begin() + count, dst.end(), 0.0f);
        frames = std::min(frames, count);
    }

    // Load once per block so a block is never processed partly in one mode.
    switch (combMode.load(std::memory_order_relaxed)) {
    case CombMode::Add:
        _process_add(frames);
        break;
    case CombMode::Root:
        _process_root(frames);
        break;
    case CombMode::Max:
        _process_max(frames);
        break;
    }

    return outputBuffer;
}

void EnvelopeCombinator::_process_add(size_t frames)
{
    // clean the output buffer
    for (auto& buffer : outputBuffer) {
        buffer.assign(buffer.size(), 0.0f);
    }

    for (int i = 0; i < numChannel; ++i) {
        for (int j = 0; j < numSource; ++j) {
            for (size_t k = 0; k < frames; ++k) {
                outputBuffer[i][k] += inputBuffer[i + j * numChannel][k];
            }
        }
    }
}

void EnvelopeCombinator::_process_root(size_t frames)
{
    // clean the output buffer
    for (auto& buffer : outputBuffer) {
        buffer.assign(buffer.size(), 0.0f);
    }

    double exponent = 1.0 / static_cast<double>(numSource);
    Sample multi_tmp;
    for (int i = 0; i < numChannel; ++i) {
        for (size_t k = 0; k < frames; ++k) {
            multi_tmp = 1;
            for (int j = 0; j < numSource; ++j) {
                multi_tmp *= inputBuffer[i + j * numChannel][k];
            }
            outputBuffer[i][k] = std::pow(std::abs(multi_tmp), exponent);
        }
    }
}

void EnvelopeCombinator::_process_max(size_t frames)
{
    // clean the output buffer
    for (auto& buffer : outputBuffer) {
        buffer.assign(buffer.size(), 0.0f);
    }

    Sample maxVal;
    Sample tmp;
    for (int i = 0; i < numChannel; ++i) {
        for (size_t k = 0; k < frames; ++k) {
            maxVal = 0;
            for (int j = 0; j < numSource; ++j) {
                tmp    = inputBuffer[i + j * numChannel][k];
                maxVal = tmp > maxVal ? tmp : maxVal;
            }
            outputBuffer[i][k] = maxVal;
        }
    }
}
