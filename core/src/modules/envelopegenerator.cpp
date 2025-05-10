/**
 * @file envelopegenerator.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Envelope Generator Class Implementation
 * @date 2024-02-07
 *
 * @copyright Copyright (c) 2023-2025
 */
#include "envelopegenerator.h"

using zerr::Blocks;
using zerr::EnvelopeGenerator;
using zerr::Param;

EnvelopeGenerator::EnvelopeGenerator(SystemConfigs systemCfgs,
    std::string speakerCfgs, Mode genMode)
{
    this->systemCfgs = systemCfgs;
    this->speakerCfgs = speakerCfgs;
    this->genMode = genMode;

    speakerManager = new SpeakerManager(this->speakerCfgs);

    logger = new Logger();

    onsetDetector = new OnsetDetector(50);

#ifdef TESTMODE
    logger->setLogLevel(LogLevel::INFO);
#endif // TESTMODE
}

bool EnvelopeGenerator::initialize()
{
    // initialize speaker manager
    if (!speakerManager->initialize())
        return false;

    // check the generator mode and bind process func
    if (genMode == "trigger") {
        processFunc = &EnvelopeGenerator::_processTrigger;
    } else if (genMode == "trajectory") {
        processFunc = &EnvelopeGenerator::_processTrajectory;
    } else {
        logger->logError(
            "EnvelopeGenerator::initialize Unknown selection mode: " + genMode);
        return false;
    }

    // get the number of speakers
    int numOutlet = speakerManager->getNumAllSpeakers();
    ;

    // initialize the inputbuffer and outputbuffer size.
    inputBuffers.resize(numInlet, Samples(systemCfgs.block_size, 0.0f));
    outputBuffers.resize(numOutlet, Samples(systemCfgs.block_size, 0.0f));

    // setup index to channel reverse lookup table
    Indexes indexes = speakerManager->getActiveSpeakerIndexes();
    for (size_t i = 0; i < indexes.size(); ++i) {
        indexChannelLookup[indexes[i]] = i;
    }

    // initialize trigger mode specified parameters
    if (genMode == "trigger") {
        speakerManager->setCurrentSpeaker(speakerManager->getRandomIndex());
        triggerMode = "random";
    }

    // initialized
    return true;
}

Blocks EnvelopeGenerator::perform(Blocks in)
{
    // fetch
    inputBuffers = in;

    // process
    if (processFunc) {
        (this->*processFunc)();
    }

    // send
    return outputBuffers;
}

int EnvelopeGenerator::getNumSpeakers()
{
    return speakerManager->getNumAllSpeakers();
}

void EnvelopeGenerator::setCurrentSpeaker(Index newIdx)
{
    speakerManager->setCurrentSpeaker(newIdx);
}

void EnvelopeGenerator::setActiveSpeakerIndexs(std::string action,
    Indexes idxs)
{
    speakerManager->setActiveSpeakers(action, idxs);
}

void EnvelopeGenerator::setTrajectoryVector(Indexes idxs)
{
    speakerManager->setTrajectoryVector(idxs);
}

void EnvelopeGenerator::setTopoMatrix(std::string action, Indexes idxs)
{
    speakerManager->setTopoMatrix(action, idxs);
}

void EnvelopeGenerator::setTriggerInterval(Param newInterval)
{
    newInterval = newInterval < 0 ? 0 : newInterval;
    int newThreshold = (int)(newInterval / 1000.0 * systemCfgs.sample_rate);
    onsetDetector->setDebounceThreshold(newThreshold);
}

void EnvelopeGenerator::printParameters() { speakerManager->printParameters(); }

EnvelopeGenerator::~EnvelopeGenerator()
{
    delete speakerManager;
    delete logger;
    delete onsetDetector;
}

void EnvelopeGenerator::setPrinter(Logger::PrintStrategy newPrinter)
{
    // The logger of EnvelopeGenerator
    logger->setPrinter(newPrinter);
    // The logger of SpeakerManger
    speakerManager->setPrinter(newPrinter);
}

void EnvelopeGenerator::_processTrigger()
{
    size_t channel;
    Params distances; // distances between speakers
    Param powerSum; // the overall power
    Param gain;
    Index currIdx;

    // empty the outputBuffers
    for (auto& buffer : outputBuffers) {
        buffer.assign(buffer.size(), 0.0f);
    }

    // input signals references
    Samples& triggr = inputBuffers[0];
    Samples& spread = inputBuffers[1];
    Samples& volume = inputBuffers[2];

    // process trigger blocks: detect onsets
    onsetDetector->detectOnsetInBlock(triggr);

    // calculate the envelopes TODO: add interpolator
    for (size_t cnt = 0; cnt < inputBuffers[0].size(); ++cnt) {
        // find main speaker
        currIdx = speakerManager->getIndexesByTrigger(triggr[cnt], triggerMode);
        channel = indexChannelLookup[currIdx]; // get the channel of the
                                               // current speaker
        outputBuffers[channel][cnt] = 1.0;
        powerSum = 1.0;

        // calculate spread gains
        distances = speakerManager->getDistanceVector(currIdx);
        for (size_t chnl = 0; chnl < outputBuffers.size(); ++chnl) {
            if (chnl == channel) {
                continue;
            }
            gain = _calculateGain(distances[chnl], spread[cnt]);
            outputBuffers[chnl][cnt] = gain * gain;
            powerSum += gain * gain;
        }

        // normalize the overall power
        for (size_t chnl = 0; chnl < outputBuffers.size(); ++chnl) {
            outputBuffers[chnl][cnt] = sqrt(outputBuffers[chnl][cnt] / powerSum) * volume[cnt];
        }
    }
}

void EnvelopeGenerator::_processTrajectory()
{
    Pair speakerPair;
    Pair channelPair;

    Param panRatio;

    Params distances;

    for (auto& buffer : outputBuffers) {
        buffer.assign(buffer.size(), 0.0f);
    }

    Samples& trjcty = inputBuffers[0];
    Samples& spread = inputBuffers[1];
    Samples& volume = inputBuffers[2];

    for (size_t cnt = 0; cnt < trjcty.size(); ++cnt) {
        speakerPair = speakerManager->getIndexesByTrajectory(trjcty[cnt]);

        channelPair.first = indexChannelLookup[speakerPair.first];
        channelPair.second = indexChannelLookup[speakerPair.second];

        if (speakerPair.first == speakerPair.second) {
            outputBuffers[channelPair.first][cnt] = volume[cnt];
        } else { // linear panning TODO: change to parameterized crossfade
            panRatio = speakerManager->getPanningRatio(trjcty[cnt]);
            outputBuffers[channelPair.first][cnt] = volume[cnt] * (1 - panRatio);
            outputBuffers[channelPair.second][cnt] = volume[cnt] * panRatio;
        }
    }
}

Param EnvelopeGenerator::_calculateGain(Param x, Param theta)
{
    x = x * DISTANCE_SCALE; // TODO: remove this distance scale or make it more
                            // versitle

    // clip the theta
    theta = theta < 0.0 ? 0 : theta;
    theta = theta > 1.0 ? 1 : theta;

    Param tmp = tan(theta * PI / 2.0);

    Param gain = isEqualTo0(tmp, VOLUME_THRESHOLD)
        ? 0.0
        : 1.0 - x / tan(theta * PI / 2.0);

    // clip gain
    gain = gain < 0.0 ? 0 : gain;
    gain = gain > 1.0 ? 1 : gain;

    return gain;
}