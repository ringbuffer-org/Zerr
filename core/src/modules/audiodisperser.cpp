#include "audiodisperser.h"
using namespace zerr;


AudioDisperser::AudioDisperser(int numChannel, zerr::SystemConfigs systemCfgs){
    this->numChannel = numChannel;
    this->systemCfgs = systemCfgs;

    numInlet  = numChannel+1;
    numOutlet = numChannel;

    logger = new Logger();
    #ifdef TESTMODE
    logger->setLogLevel(LogLevel::INFO);
    #endif //TESTMODE
}

bool AudioDisperser::initialize(){
    inputBuffer.resize(numInlet,   Samples(systemCfgs.block_size, 0.0f));
    outputBuffer.resize(numOutlet, Samples(systemCfgs.block_size, 0.0f));

    return true;
}


void AudioDisperser::fetch(Blocks in){
    inputBuffer = in;
}


void AudioDisperser::process(){
    // clean the output buffer
    for (auto& buffer : outputBuffer) {
        buffer.assign(buffer.size(), 0.0f);
    }

    Samples& source =  inputBuffer[0];

    for (int i = 1; i < numInlet; ++i) {
        for (size_t j = 0; j < systemCfgs.block_size; ++j) {
            outputBuffer[i-1][j] = source[j] * inputBuffer[i][j];
        }
    }

}


Blocks AudioDisperser::send(){
    return outputBuffer;
}

