/**
 * @file zerr_disperser.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief AudioDisperser Class Puredata Wrapper
 * @date 2024-01-29
 *
 * @copyright Copyright (c) 2023-2025
 */
#include "zerr_disperser.h"

ZerrDisperser::ZerrDisperser(int numChannel, zerr::SystemConfigs systemCfgs)
{
    audioDisperser = new zerr::AudioDisperser(numChannel, systemCfgs);

    logger = new zerr::Logger();

#ifdef TESTMODE
    logger->setLogLevel(zerr::LogLevel::INFO);
#endif // TESTMODE
}

bool ZerrDisperser::initialize()
{
    if (!audioDisperser->initialize()) {
        return false;
    }
    numInlet  = audioDisperser->numInlet;
    numOutlet = audioDisperser->numOutlet;

    inputBuffer.assign(numInlet, zerr::Samples(audioDisperser->get_block_size(), 0.0f));
    outputBuffer.assign(numOutlet, zerr::Samples(audioDisperser->get_block_size(), 0.0f));

#ifdef TESTMODE
    logger->logDebug(
        zerr::formatString("ZerrDisperser::initialize numInlet:%d numOutlet:%d blockSize %d",
                           numInlet, numOutlet, audioDisperser->get_block_size()));
#endif // TESTMODE

    inPtr  = (float**)getbytes(numInlet * sizeof(float**));
    outPtr = (float**)getbytes(numOutlet * sizeof(float**));

    return true;
}

bool ZerrDisperser::prepare(zerr::SystemConfigs cfg)
{
    if (!audioDisperser->prepare(cfg)) {
        return false;
    }
    inputBuffer.assign(numInlet, zerr::Samples(cfg.block_size, 0.0f));
    return true;
}

void ZerrDisperser::perform(float** ports, int blockSize)
{
    inPtr  = (float**)&ports[0];
    outPtr = (float**)&ports[numInlet];

    // never read or write past the prepared block; the remainder of the output is silence
    const int block = audioDisperser->get_block_size();
    const int count = blockSize < block ? blockSize : block;

    for (int i = 0; i < numInlet; i++) {
        for (int j = 0; j < count; j++) {
            inputBuffer[i][j] = inPtr[i][j];
        }
        for (int j = count; j < block; j++) {
            inputBuffer[i][j] = 0.0;
        }
    }

    const zerr::Blocks& out = audioDisperser->perform(inputBuffer);

    for (int i = 0; i < numOutlet; i++) {
        const int have = (int)out[i].size() < count ? (int)out[i].size() : count;
        for (int j = 0; j < have; j++) {
            outPtr[i][j] = out[i][j];
        }
        for (int j = have; j < blockSize; j++) {
            outPtr[i][j] = 0.0f;
        }
    }
}

ZerrDisperser::~ZerrDisperser()
{
    delete audioDisperser;
    delete logger;
}
