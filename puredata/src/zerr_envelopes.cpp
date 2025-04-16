/**
 * @file zerr_envelopes.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Envelope Generator Class Puredata wrapper
 * @date 2024-01-31
 *
 * @copyright Copyright (c) 2023-2025
 */
#include "zerr_envelopes.h"

ZerrEnvelopes::ZerrEnvelopes(zerr::SystemConfigs systemCfgs, std::string selectionMode, std::string spkrCfgFile)
{
    this->systemCfgs = systemCfgs;
    this->spkrCfgFile = spkrCfgFile;
    this->selectionMode = selectionMode;

    envelopeGenerator = new zerr::EnvelopeGenerator(systemCfgs, spkrCfgFile, selectionMode);
}

bool ZerrEnvelopes::initialize()
{
    // Initialize the Envelope Generator module
    if (!envelopeGenerator->initialize()) {
        return false;
    };

    // assgin Pure Data print method the the logger function
    // Because we want to see logs in the PD log window
    auto printFunc = [](const std::string& msg) {
        post(msg.c_str());
    };
    envelopeGenerator->setPrinter(printFunc);

    // post("1. About to call setPrinter");
    // envelopeGenerator->logger->logInfo("2. About to call setPrinter");
    // envelopeGenerator->speakerManager->logger->logInfo("3. About to call setPrinter");

    numOutlet = envelopeGenerator->getNumSpeakers();

    inputBuffer.resize(numInlet, zerr::Samples(systemCfgs.block_size, 0.0f));
    outputBuffer.resize(numOutlet, zerr::Samples(systemCfgs.block_size, 0.0f));

    inPtr = (float**)getbytes(numInlet * sizeof(float**));
    outPtr = (float**)getbytes(numOutlet * sizeof(float**));

    return true;
}

void ZerrEnvelopes::perform(float** ports, int blockSize)
{
    inPtr = (float**)&ports[0];
    outPtr = (float**)&ports[numInlet];

    for (int i = 0; i < numInlet; i++) {
        for (int j = 0; j < blockSize; j++) {
            inputBuffer[i][j] = inPtr[i][j];
        }
    }

    outputBuffer = envelopeGenerator->perform(inputBuffer);

    for (int i = 0; i < numOutlet; i++) {
        for (int j = 0; j < blockSize; j++) {
            outPtr[i][j] = outputBuffer[i][j];
        }
    }
}

int ZerrEnvelopes::get_port_count()
{
    return numInlet + numOutlet;
}

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

void ZerrEnvelopes::printParameters()
{
    envelopeGenerator->printParameters();
}

ZerrEnvelopes::~ZerrEnvelopes()
{
    delete envelopeGenerator;
}
