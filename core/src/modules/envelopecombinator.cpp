/**
 * @file envelopecombinator.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Envelope Combinator Class Implementation
 * @date 2024-02-03
 *
 * @copyright Copyright (c) 2023-2024
 */
#include "envelopecombinator.h"
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
    inputBuffer.resize(numInlet, Samples(systemCfgs.block_size, 0.0f));
    outputBuffer.resize(numOutlet, Samples(systemCfgs.block_size, 0.0f));

    if (!modeValid) {
        logger.logError("EnvelopeCombinator::initialize Unknown combination mode: " +
                        requestedModeName);
        return false;
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
    if (!prepared) {
        return outputBuffer;
    }

    inputBuffer = in;

    // Load once per block so a block is never processed partly in one mode.
    switch (combMode.load(std::memory_order_relaxed)) {
    case CombMode::Add:
        _process_add();
        break;
    case CombMode::Root:
        _process_root();
        break;
    case CombMode::Max:
        _process_max();
        break;
    }

    return outputBuffer;
}

void EnvelopeCombinator::_process_add()
{
    // clean the output buffer
    for (auto& buffer : outputBuffer) {
        buffer.assign(buffer.size(), 0.0f);
    }

    for (int i = 0; i < numChannel; ++i) {
        for (int j = 0; j < numSource; ++j) {
            for (size_t k = 0; k < systemCfgs.block_size; ++k) {
                outputBuffer[i][k] += inputBuffer[i + j * numChannel][k];
            }
        }
    }
}

void EnvelopeCombinator::_process_root()
{
    // clean the output buffer
    for (auto& buffer : outputBuffer) {
        buffer.assign(buffer.size(), 0.0f);
    }

    double exponent = 1.0 / static_cast<double>(numSource);
    Sample multi_tmp;
    for (int i = 0; i < numChannel; ++i) {
        // TODO: use systemcfg.block_size could cause bug(sometimes smaller)
        for (size_t k = 0; k < systemCfgs.block_size; ++k) {
            multi_tmp = 1;
            for (int j = 0; j < numSource; ++j) {
                multi_tmp *= inputBuffer[i + j * numChannel][k];
            }
            outputBuffer[i][k] = std::pow(std::abs(multi_tmp), exponent);
        }
    }
}

void EnvelopeCombinator::_process_max()
{
    // clean the output buffer
    for (auto& buffer : outputBuffer) {
        buffer.assign(buffer.size(), 0.0f);
    }

    Sample maxVal;
    Sample tmp;
    for (int i = 0; i < numChannel; ++i) {
        for (size_t k = 0; k < systemCfgs.block_size; ++k) {
            maxVal = 0;
            for (int j = 0; j < numSource; ++j) {
                tmp    = inputBuffer[i + j * numChannel][k];
                maxVal = tmp > maxVal ? tmp : maxVal;
            }
            outputBuffer[i][k] = maxVal;
        }
    }
}
