/**
 * @file zerr_disperser.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief AudioDisperser Class Puredata Wrapper
 * @date 2024-01-29
 * 
 * @copyright Copyright (c) 2023-2024
 */
#include "zerr_disperser.h"


ZerrDisperser::ZerrDisperser(int numChannel, zerr::SystemConfigs systemCfgs) {
    audioDisperser = new zerr::AudioDisperser(numChannel, systemCfgs);
    logger = new zerr::Logger();

    #ifdef TESTMODE
    logger->setLogLevel(zerr::LogLevel::INFO);
    #endif  // TESTMODE
}


bool ZerrDisperser::initialize() {
    if (!audioDisperser->initialize()) {return false;}
    numInlet  = audioDisperser->numInlet;
    numOutlet = audioDisperser->numOutlet;

    inputBuffer.resize(numInlet,
        zerr::Samples(audioDisperser->get_block_size(), 0.0f));
    outputBuffer.resize(numOutlet,
        zerr::Samples(audioDisperser->get_block_size(), 0.0f));

    #ifdef TESTMODE
    logger->logDebug(zerr::formatString(
        "ZerrDisperser::initialize numInlet:%d numOutlet:%d blockSize %d",
        numInlet, numOutlet, audioDisperser->get_block_size()));
    #endif  // TESTMODE

    inPtr  = (float **) getbytes(numInlet * sizeof(float **));
    outPtr = (float **) getbytes(numOutlet * sizeof(float **));

    return true;
}


void ZerrDisperser::perform(float **ports, int blockSize) {
    inPtr  = (float **) &ports[0];
    outPtr = (float **) &ports[numInlet];

    for (int i = 0; i < numInlet; i++) {
        for (int j = 0; j < blockSize; j++) {
            inputBuffer[i][j] = inPtr[i][j];
        }
    }

    try {
        audioDisperser->fetch(inputBuffer);
        audioDisperser->process();
        outputBuffer = audioDisperser->send();
    }
    catch (...) {
        // logger->logError("ZerrDisperser::perform process failed...");
        return;
    }

    for (int i = 0; i < numOutlet; i++) {
        for (int j = 0; j < blockSize; j++) {
            outPtr[i][j] = outputBuffer[i][j];
        }
    }
}


ZerrDisperser::~ZerrDisperser() {
    delete audioDisperser;
    delete logger;
}
