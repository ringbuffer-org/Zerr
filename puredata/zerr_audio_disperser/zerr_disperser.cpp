#include "zerr_disperser.h"


ZerrAudioDisperser::ZerrAudioDisperser(int numChannel, zerr::t_systemConfigs systemCfgs){
    audioDisperser = new zerr::AudioDisperser(numChannel, systemCfgs);
    logger = new zerr::Logger();
    #ifdef TESTMODE
    logger->setLogLevel(zerr::LogLevel::INFO);
    #endif //TESTMODE
}


bool ZerrAudioDisperser::initialize(){
    if (!audioDisperser->initialize()) {return false;};
    numInlet  = audioDisperser->numInlet;
    numOutlet = audioDisperser->numOutlet;

    inputBuffer.resize(numInlet,   zerr::t_samples(audioDisperser->get_block_size(), 0.0f));
    outputBuffer.resize(numOutlet, zerr::t_samples(audioDisperser->get_block_size(), 0.0f));

    logger->logDebug(zerr::formatString("ZerrAudioDisperser::initialize numInlet:%d numOutlet:%d blockSize %d", numInlet, numOutlet, audioDisperser->get_block_size()));

    inPtr  = (float **) getbytes(numInlet * sizeof(float **));
    outPtr = (float **) getbytes(numOutlet * sizeof(float **));

    logger->logInfo("ZerrAudioDisperser::initialize initialized");
    return true;
}


void ZerrAudioDisperser::perform(float **ports, int blockSize){
    inPtr  = (float **) &ports[0];
    outPtr = (float **) &ports[numInlet];
    // length(outPtr);
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
        // logger->logError("ZerrAudioDisperser::perform process failed...");
        return;
    }

    for (int i = 0; i < numOutlet; i++) {
        for (int j = 0; j < blockSize; j++) {
            outPtr[i][j] = outputBuffer[i][j];
        }
    }
}


ZerrAudioDisperser::~ZerrAudioDisperser(){
    delete audioDisperser;
    delete logger;
}
