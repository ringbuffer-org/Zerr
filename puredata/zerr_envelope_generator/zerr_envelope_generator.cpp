#include "zerr_envelope_generator.h"



ZerrEnvelopeGenerator::ZerrEnvelopeGenerator(zerr::t_systemConfigs systemCfgs, std::string selectionMode, std::string spkrCfgFile){
    this->systemCfgs    = systemCfgs;
    this->spkrCfgFile   = spkrCfgFile;
    this->selectionMode = selectionMode;

    envelopeGenerator = new zerr::EnvelopeGenerator(systemCfgs, spkrCfgFile, selectionMode);
    logger = new zerr::Logger();
    #ifdef TESTMODE
    logger->setLogLevel(zerr::LogLevel::INFO);
    #endif //TESTMODE
}
 

bool ZerrEnvelopeGenerator::initialize(){
    if (!envelopeGenerator->initialize()) {return false;};
    numOutlet = envelopeGenerator->get_n_speaker();

    inputBuffer.resize(numInlet,   zerr::t_samples(systemCfgs.block_size, 0.0f));
    outputBuffer.resize(numOutlet, zerr::t_samples(systemCfgs.block_size, 0.0f));

    inPtr  = (float **) getbytes(numInlet * sizeof(float **));
    outPtr = (float **) getbytes(numOutlet * sizeof(float **));

    logger->logInfo("ZerrEnvelopeGenerator::initialize initialized");
    return true;
}


void ZerrEnvelopeGenerator::perform(float **ports, int blockSize){
    inPtr  = (float **) &ports[0];
    outPtr = (float **) &ports[numInlet];

    for (int i = 0; i < numInlet; i++) {
        for (int j = 0; j < blockSize; j++) {
            inputBuffer[i][j] = inPtr[i][j];
        }
    }

    try {
        envelopeGenerator->fetch(inputBuffer); 
        envelopeGenerator->process();
        outputBuffer = envelopeGenerator->send();
    }
    catch (...) {
        // logger->logError("ZerrEnvelopeGenerator::perform process failed...");
        return;
    }

    for (int i = 0; i < numOutlet; i++) {
        for (int j = 0; j < blockSize; j++) {
            outPtr[i][j] = outputBuffer[i][j];
        }
    }
}


int ZerrEnvelopeGenerator::get_port_count(){
    return numInlet + numOutlet;
}


void ZerrEnvelopeGenerator::set_unmasked_indexs(int * idxs, size_t size){
    zerr::t_indexs indexVec(idxs, idxs + size);
    envelopeGenerator->set_unmasked_indexs(indexVec, "set");
}


ZerrEnvelopeGenerator::~ZerrEnvelopeGenerator(){
    delete envelopeGenerator;
    delete logger;
}
