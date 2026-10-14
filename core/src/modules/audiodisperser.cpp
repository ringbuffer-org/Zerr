/**
 * @file audiodisperser.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief AudioDisperser Class Implementation
 * @date 2024-02-06
 *
 * @copyright Copyright (c) 2023-2024
 */

#include "audiodisperser.h"

#include <algorithm>
using namespace zerr;

AudioDisperser::AudioDisperser(int numChannel, zerr::SystemConfigs systemCfgs)
    : numChannel(numChannel), systemCfgs(systemCfgs)
{
    numInlet  = numChannel + 1;
    numOutlet = numChannel;

#ifdef TESTMODE
    logger.setLogLevel(LogLevel::INFO);
#endif // TESTMODE
}

bool AudioDisperser::initialize()
{
    inputBuffer.assign(numInlet, Samples());
    outputBuffer.assign(numOutlet, Samples());
    const SystemConfigs cfg = systemCfgs;
    systemCfgs              = SystemConfigs{0, 0}; // fresh buffers are never "already prepared"
    if (!prepare(cfg)) {
        logger.logError("AudioDisperser::initialize block size and sample rate must be > 0");
        return false;
    }
    return true;
}

bool AudioDisperser::prepare(SystemConfigs cfg)
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
    return true;
}

const Blocks& AudioDisperser::perform(const Blocks& in)
{
    // clean the output buffer
    for (auto& buffer : outputBuffer) {
        buffer.assign(buffer.size(), 0.0f);
    }

    if (systemCfgs.block_size == 0 || static_cast<int>(in.size()) < numInlet) {
        return outputBuffer; // not prepared, or missing input: silence
    }

    // Copy into the prepared blocks, never past them; the frame count is the shortest input
    size_t frames = systemCfgs.block_size;
    for (int i = 0; i < numInlet; ++i) {
        Samples& dst       = inputBuffer[i];
        const size_t count = std::min(in[i].size(), dst.size());
        std::copy_n(in[i].begin(), count, dst.begin());
        std::fill(dst.begin() + count, dst.end(), 0.0f);
        frames = std::min(frames, count);
    }

    const Samples& source = inputBuffer[0];
    for (int i = 1; i < numInlet; ++i) {
        for (size_t j = 0; j < frames; ++j) {
            outputBuffer[i - 1][j] = source[j] * inputBuffer[i][j];
        }
    }

    return outputBuffer;
}
