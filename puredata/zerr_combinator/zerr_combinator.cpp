/**
 * @file zerr_combinator.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief EnvelopeCombinator Class Puredata Wrapper
 * @date 2024-01-29
 * 
 * @copyright Copyright (c) 2023-2024
 */
#include "./zerr_combinator.h"

// this one was needed to compile on linux
// (error: ‘strdup’ was not declared in this scope)
char* strdup(const char* s) {
  size_t slen = strlen(s);
  char* result = (char*) malloc(slen + 1);
  if (result == NULL) {
    return NULL;
  }

  memcpy(result, s, slen+1);
  return result;
}

ZerrCombinator::ZerrCombinator(int numSource, int numChannel, std::string combinationMode, zerr::t_systemConfigs systemCfgs) {
    envelopeCombinator = new zerr::EnvelopeCombinator(numSource, numChannel, systemCfgs, combinationMode);
    logger = new zerr::Logger();
    #ifdef TESTMODE
    logger->setLogLevel(zerr::LogLevel::INFO);
    #endif  // TESTMODE
}


bool ZerrCombinator::initialize() {
    if (!envelopeCombinator->initialize()) {return false;}
    numInlet  = envelopeCombinator->numInlet;
    numOutlet = envelopeCombinator->numOutlet;

    inputBuffer.resize(numInlet,
        zerr::t_samples(envelopeCombinator->get_block_size(), 0.0f));
    outputBuffer.resize(numOutlet,
        zerr::t_samples(envelopeCombinator->get_block_size(), 0.0f));

    logger->logDebug(zerr::formatString(
        "ZerrCombinator::initialize numInlet:%d numOutlet:%d blockSize %d",
        numInlet, numOutlet, envelopeCombinator->get_block_size()));

    inPtr  = (float **) getbytes(numInlet * sizeof(float **));
    outPtr = (float **) getbytes(numOutlet * sizeof(float **));

    logger->logInfo("ZerrCombinator::initialize initialized");
    return true;
}


void ZerrCombinator::perform(float **ports, int blockSize) {
    inPtr  = (float **) &ports[0];
    outPtr = (float **) &ports[numInlet];

    for (int i = 0; i < numInlet; i++) {
        for (int j = 0; j < blockSize; j++) {
            inputBuffer[i][j] = inPtr[i][j];
        }
    }

    try {
        envelopeCombinator->fetch(inputBuffer);
        envelopeCombinator->process();
        outputBuffer = envelopeCombinator->send();
    }
    catch (...) {
        return;
    }

    for (int i = 0; i < numOutlet; i++) {
        for (int j = 0; j < blockSize; j++) {
            outPtr[i][j] = outputBuffer[i][j];
        }
    }
}


ZerrCombinator::~ZerrCombinator() {
    delete envelopeCombinator;
    delete logger;
}
