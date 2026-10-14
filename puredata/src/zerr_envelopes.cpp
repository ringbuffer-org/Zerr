/**
 * @file zerr_envelopes.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Envelope Generator Class Puredata wrapper
 * @date 2024-01-31
 *
 * @copyright Copyright (c) 2023-2025
 */
#include "zerr_envelopes.h"

ZerrEnvelopes::ZerrEnvelopes(zerr::SystemConfigs systemCfgs, std::string selectionMode,
                             std::string spkrCfgFile)
{
    this->systemCfgs    = systemCfgs;
    this->spkrCfgFile   = spkrCfgFile;
    this->selectionMode = selectionMode;

    envelopeGenerator =
        new zerr::EnvelopeGenerator(systemCfgs, spkrCfgFile, zerr::parseGenMode(selectionMode));
}

bool ZerrEnvelopes::initialize()
{
    // Initialize the Envelope Generator module
    if (!envelopeGenerator->initialize()) {
        return false;
    };

    // assgin Max/MSP post method the the logger function
    // Because we want to see logs in the PD log window
    auto printFunc = [](const std::string& msg) { post(msg.c_str()); };
    envelopeGenerator->setPrinter(printFunc);

    numOutlet = envelopeGenerator->getNumSpeakers();

    inputBuffer.assign(numInlet, zerr::Samples(systemCfgs.block_size, 0.0f));
    outputBuffer.assign(numOutlet, zerr::Samples(systemCfgs.block_size, 0.0f));

    inPtr  = (float**)getbytes(numInlet * sizeof(float**));
    outPtr = (float**)getbytes(numOutlet * sizeof(float**));

    return true;
}

bool ZerrEnvelopes::prepare(zerr::SystemConfigs cfg)
{
    if (!envelopeGenerator->prepare(cfg)) {
        return false;
    }
    systemCfgs = cfg;
    inputBuffer.assign(numInlet, zerr::Samples(cfg.block_size, 0.0f));
    return true;
}

void ZerrEnvelopes::perform(float** ports, int blockSize)
{
    inPtr  = (float**)&ports[0];
    outPtr = (float**)&ports[numInlet];

    // never read or write past the prepared block; the remainder of the output is silence
    const int block = (int)systemCfgs.block_size;
    const int count = blockSize < block ? blockSize : block;

    for (int i = 0; i < numInlet; i++) {
        for (int j = 0; j < count; j++) {
            inputBuffer[i][j] = inPtr[i][j];
        }
        for (int j = count; j < block; j++) {
            inputBuffer[i][j] = 0.0;
        }
    }

    outputBuffer = envelopeGenerator->perform(inputBuffer);

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

int ZerrEnvelopes::get_port_count() { return numInlet + numOutlet; }

void ZerrEnvelopes::setActiveSpeakerIndexs(char* action, int* idxs, size_t size)
{
    zerr::Indexes indexVec(idxs, idxs + size);
    envelopeGenerator->setActiveSpeakerIndexs(action, indexVec);
}

void ZerrEnvelopes::setCurrentSpeaker(int idx)
{
    zerr::Index newIdx = idx;
    envelopeGenerator->setCurrentSpeaker(newIdx);
}

void ZerrEnvelopes::setTopoMatrix(char* action, int* idxs, size_t size)
{
    zerr::Indexes indexVec(idxs, idxs + size);
    envelopeGenerator->setTopoMatrix(action, indexVec);
}

void ZerrEnvelopes::setTrajectoryVector(int* idxs, size_t size)
{
    zerr::Indexes indexVec(idxs, idxs + size);
    envelopeGenerator->setTrajectoryVector(indexVec);
}

void ZerrEnvelopes::setTriggerInterval(float interval)
{
    zerr::Param newInterval = interval;
    envelopeGenerator->setTriggerInterval(newInterval);
}

void ZerrEnvelopes::printParameters() { envelopeGenerator->printParameters(); }

ZerrEnvelopes::~ZerrEnvelopes() { delete envelopeGenerator; }
