/**
 * @file zerr_combinator.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief EnvelopeCombinator Class Puredata Wrapper
 * @date 2024-01-29
 *
 * @copyright Copyright (c) 2023-2025
 */
#include "./zerr_combinator.h"

// this one was needed to compile on linux
// (error: ‘strdup’ was not declared in this scope)
char* strdup(const char* s)
{
    size_t slen  = strlen(s);
    char* result = (char*)malloc(slen + 1);
    if (result == NULL) {
        return NULL;
    }

    memcpy(result, s, slen + 1);
    return result;
}

ZerrCombinator::ZerrCombinator(int numSource, int numChannel, std::string combinationMode,
                               zerr::SystemConfigs systemCfgs)
{
    envelopeCombinator =
        new zerr::EnvelopeCombinator(numSource, numChannel, systemCfgs, combinationMode);
    logger = new zerr::Logger();
#ifdef TESTMODE
    logger->setLogLevel(zerr::LogLevel::INFO);
#endif // TESTMODE
}

bool ZerrCombinator::initialize()
{
    if (!envelopeCombinator->initialize()) {
        return false;
    }
    numInlet  = envelopeCombinator->numInlet;
    numOutlet = envelopeCombinator->numOutlet;

    inputBuffer.assign(numInlet, zerr::Samples(envelopeCombinator->get_block_size(), 0.0f));
    outputBuffer.assign(numOutlet, zerr::Samples(envelopeCombinator->get_block_size(), 0.0f));

    logger->logDebug(
        zerr::formatString("ZerrCombinator::initialize numInlet:%d numOutlet:%d blockSize %d",
                           numInlet, numOutlet, envelopeCombinator->get_block_size()));

    inPtr  = (float**)getbytes(numInlet * sizeof(float**));
    outPtr = (float**)getbytes(numOutlet * sizeof(float**));

    logger->logInfo("ZerrCombinator::initialize initialized");
    return true;
}

bool ZerrCombinator::prepare(zerr::SystemConfigs cfg)
{
    if (!envelopeCombinator->prepare(cfg)) {
        return false;
    }
    inputBuffer.assign(numInlet, zerr::Samples(cfg.block_size, 0.0f));
    return true;
}

void ZerrCombinator::perform(float** ports, int blockSize)
{
    inPtr  = (float**)&ports[0];
    outPtr = (float**)&ports[numInlet];

    // never read or write past the prepared block; the remainder of the output is silence
    const int block = envelopeCombinator->get_block_size();
    const int count = blockSize < block ? blockSize : block;

    for (int i = 0; i < numInlet; i++) {
        for (int j = 0; j < count; j++) {
            inputBuffer[i][j] = inPtr[i][j];
        }
        for (int j = count; j < block; j++) {
            inputBuffer[i][j] = 0.0;
        }
    }

    outputBuffer = envelopeCombinator->perform(inputBuffer);

    for (int i = 0; i < numOutlet; i++) {
        const int have = (int)outputBuffer[i].size() < count ? (int)outputBuffer[i].size() : count;
        for (int j = 0; j < have; j++) {
            outPtr[i][j] = outputBuffer[i][j];
        }
        for (int j = have; j < blockSize; j++) {
            outPtr[i][j] = 0.0f;
        }
    }
}

ZerrCombinator::~ZerrCombinator()
{
    delete envelopeCombinator;
    delete logger;
}
