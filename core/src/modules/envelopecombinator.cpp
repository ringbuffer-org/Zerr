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

EnvelopeCombinator::EnvelopeCombinator(int numSource, int numChannel, t_systemConfigs systemCfgs, std::string combMode){
    this->numSource  = numSource;
    this->numChannel = numChannel;
    this->systemCfgs = systemCfgs;
    this->combMode = combMode;

    numInlet = numSource * numChannel;
    numOutlet = numChannel;

    logger = new Logger();
    #ifdef TESTMODE
    logger->setLogLevel(LogLevel::INFO);
    #endif //TESTMODE
}

bool EnvelopeCombinator::initialize(){
    inputBuffer.resize(numInlet,   t_samples(systemCfgs.block_size, 0.0f));
    outputBuffer.resize(numOutlet, t_samples(systemCfgs.block_size, 0.0f));

    if (combMode!="add" && 
        combMode!="root" && 
        combMode!="max"){ //TODO: change to enumerate and add to type.h
        logger->logError(
            "EnvelopeCombinator::initialize Unknown combination mode: " + combMode);
        return false;
    }

    if (combMode == "add") processFunc = &EnvelopeCombinator::_process_add;
    else if (combMode == "root") processFunc = &EnvelopeCombinator::_process_root;
    else if (combMode == "max") processFunc = &EnvelopeCombinator::_process_max;

    return true;
}


void EnvelopeCombinator::fetch(BlockIns in){
    inputBuffer = in;
}


void EnvelopeCombinator::process(){
    if (processFunc) {
        (this->*processFunc)();
    }
}


void EnvelopeCombinator::_process_add(){
    // clean the output buffer
    for (auto& buffer : outputBuffer) {
        buffer.assign(buffer.size(), 0.0f);
    }

    for (int i = 0; i < numChannel; ++i) {
        for (int j = 0; j < numSource; ++j) {
            for (size_t k = 0; k < systemCfgs.block_size; ++k) {
                outputBuffer[i][k] += inputBuffer[i + j*numChannel][k];
            }
        }
    }
}


void EnvelopeCombinator::_process_root(){
    // clean the output buffer
    for (auto& buffer : outputBuffer) {
        buffer.assign(buffer.size(), 0.0f);
    }

    double exponent = 1.0 / (double) numSource;
    t_sample multi_tmp;
    for (int i = 0; i < numChannel; ++i) {
        // TODO: use systemcfg.block_size could cause bug(sometimes smaller)
        for (size_t k = 0; k < systemCfgs.block_size; ++k) {
            multi_tmp = 1;
            for (int j = 0; j < numSource; ++j) {
                multi_tmp *= inputBuffer[i + j*numChannel][k];
            }
            outputBuffer[i][k] = std::pow(std::abs(multi_tmp), exponent);
        }
    }
}


void EnvelopeCombinator::_process_max(){
    // clean the output buffer
    for (auto& buffer : outputBuffer) {
        buffer.assign(buffer.size(), 0.0f);
    }

    t_sample maxVal;
    t_sample tmp;
    for (int i = 0; i < numChannel; ++i) {
        for (size_t k = 0; k < systemCfgs.block_size; ++k) {
            maxVal = 0;
            for (int j = 0; j < numSource; ++j) {
                tmp = inputBuffer[i + j*numChannel][k];
                maxVal = tmp>maxVal?tmp:maxVal;
            }
            outputBuffer[i][k] = maxVal;
        }
    }
}


t_blockOuts EnvelopeCombinator::send(){
    return outputBuffer;
}

EnvelopeCombinator::~EnvelopeCombinator() {
    delete logger;
}
