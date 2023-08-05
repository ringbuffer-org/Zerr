#include "zerr_envelope_combinator.h"


ZerrEnvelopeCombinator::ZerrEnvelopeCombinator(int numSource, int numChannel, zerr::t_systemConfigs systemCfgs, std::string combinationMode){
    envelopeCombinator = new zerr::EnvelopeCombinator(numSource, numChannel, systemCfgs, combinationMode);
    logger = new zerr::Logger();
    #ifdef TESTMODE
    logger->setLogLevel(zerr::LogLevel::INFO);
    #endif //TESTMODE
}


bool ZerrEnvelopeCombinator::initialize(){
    if (!envelopeCombinator->initialize()) {return false;};
    numInlet  = envelopeCombinator->numInlet;
    numOutlet = envelopeCombinator->numOutlet;

    inputBuffer.resize(numInlet,   zerr::t_samples(envelopeCombinator->get_block_size(), 0.0f));
    outputBuffer.resize(numOutlet, zerr::t_samples(envelopeCombinator->get_block_size(), 0.0f));

    logger->logDebug(zerr::formatString("ZerrEnvelopeCombinator::initialize numInlet:%d numOutlet:%d blockSize %d", numInlet, numOutlet, envelopeCombinator->get_block_size()));

    inPtr  = (float **) getbytes(numInlet * sizeof(float **));
    outPtr = (float **) getbytes(numOutlet * sizeof(float **));

    logger->logInfo("ZerrEnvelopeCombinator::initialize initialized");
    return true;
}


void ZerrEnvelopeCombinator::perform(float **ports, int blockSize){
    inPtr  = (float **) &ports[0];
    outPtr = (float **) &ports[numInlet];
    // length(outPtr);
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
        // logger->logError("ZerrEnvelopeCombinator::perform process failed...");
        return;
    }

    for (int i = 0; i < numOutlet; i++) {
        for (int j = 0; j < blockSize; j++) {
            outPtr[i][j] = outputBuffer[i][j];
        }
    }
}


// int ZerrEnvelopeCombinator::get_port_count(){
//     return numInlet + numOutlet;
// }


ZerrEnvelopeCombinator::~ZerrEnvelopeCombinator(){
    // delete envelopeCombinator;
    delete logger;
}
