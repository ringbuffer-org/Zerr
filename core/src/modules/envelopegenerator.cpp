/**
 * @file envelopegenerator.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Envelope Generator Class Implementation
 * @date 2024-02-07
 *
 * @copyright Copyright (c) 2023-2025
 */
#include "envelopegenerator.h"

#include <algorithm>

using zerr::Blocks;
using zerr::DEFAULT_ONSET_DEBOUNCE;
using zerr::DISTANCE_SCALE;
using zerr::EnvelopeGenerator;
using zerr::Param;
using zerr::pi;
using zerr::VOLUME_THRESHOLD;

EnvelopeGenerator::EnvelopeGenerator(SystemConfigs systemCfgs, std::string speakerCfgs,
                                     GenMode genMode)
    : systemCfgs(systemCfgs), speakerCfgs(speakerCfgs), genMode(genMode),
      speakerManager(std::make_unique<SpeakerManager>(speakerCfgs)),
      onsetDetector(std::make_unique<OnsetDetector>(DEFAULT_ONSET_DEBOUNCE))
{
#ifdef TESTMODE
    logger.setLogLevel(LogLevel::INFO);
#endif // TESTMODE
}

bool EnvelopeGenerator::initialize()
{
    // initialize speaker manager
    if (!speakerManager->initialize())
        return false;

    // check the generator mode and bind process func
    switch (genMode) {
    case GenMode::Trigger:
        processFunc = &EnvelopeGenerator::_processTrigger;
        break;
    case GenMode::Trajectory:
        processFunc = &EnvelopeGenerator::_processTrajectory;
        break;
    }

    // get the number of speakers (the public member, not a shadowing local)
    numOutlet = speakerManager->getNumAllSpeakers();

    // initialize the inputbuffer and outputbuffer size.
    inputBuffers.resize(numInlet, Samples(systemCfgs.block_size, 0.0f));
    outputBuffers.resize(numOutlet, Samples(systemCfgs.block_size, 0.0f));

    // setup index to channel reverse lookup table
    Indexes indexes = speakerManager->getActiveSpeakerIndexes();
    for (size_t i = 0; i < indexes.size(); ++i) {
        indexChannelLookup[indexes[i]] = i;
    }

    // initialize trigger mode specified parameters
    if (genMode == GenMode::Trigger) {
        speakerManager->setCurrentSpeaker(speakerManager->getRandomIndex());
        triggerMode = TriggerMode::Random;
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

int EnvelopeGenerator::getNumSpeakers() { return speakerManager->getNumAllSpeakers(); }

void EnvelopeGenerator::setCurrentSpeaker(Index newIdx)
{
    speakerManager->setCurrentSpeaker(newIdx);
}

void EnvelopeGenerator::setActiveSpeakerIndexs(std::string action, Indexes idxs)
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
    newInterval      = newInterval < 0 ? 0 : newInterval;
    int newThreshold = static_cast<int>(newInterval / 1000.0 * systemCfgs.sample_rate);
    onsetDetector->setDebounceThreshold(newThreshold);
}

void EnvelopeGenerator::printParameters() { speakerManager->printParameters(); }

void EnvelopeGenerator::setPrinter(Logger::PrintStrategy newPrinter)
{
    // The logger of EnvelopeGenerator
    logger.setPrinter(newPrinter);
    // The logger of SpeakerManger
    speakerManager->setPrinter(newPrinter);
}

void EnvelopeGenerator::setLogLevel(LogLevel level)
{
    logger.setLogLevel(level);
    speakerManager->logger.setLogLevel(level);
}

void EnvelopeGenerator::setSpeakerConfigText(std::string yamlText)
{
    speakerManager->setConfigText(std::move(yamlText));
}

void EnvelopeGenerator::setSystemConfigs(SystemConfigs newCfgs)
{
    systemCfgs = newCfgs;
    for (auto& block : inputBuffers) {
        block.assign(systemCfgs.block_size, 0.0f);
    }
    for (auto& block : outputBuffers) {
        block.assign(systemCfgs.block_size, 0.0f);
    }
}

void EnvelopeGenerator::_processTrigger()
{
    size_t channel;
    Params distances; // distances between speakers
    Param powerSum;   // the overall power
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

    // never write past the shorter of the input and output blocks
    const size_t frames =
        std::min(triggr.size(), outputBuffers.empty() ? size_t{0} : outputBuffers[0].size());

    // calculate the envelopes TODO: add interpolator
    for (size_t cnt = 0; cnt < frames; ++cnt) {
        // find main speaker
        currIdx = speakerManager->getIndexesByTrigger(triggr[cnt], triggerMode);
        // find, not operator[]: never insert into the lookup from the audio thread, and a
        // speaker without a channel (deactivated at runtime) leaves this frame silent
        auto lookup = indexChannelLookup.find(currIdx);
        if (lookup == indexChannelLookup.end() || lookup->second >= outputBuffers.size()) {
            continue;
        }
        channel                     = lookup->second;
        outputBuffers[channel][cnt] = 1.0;
        powerSum                    = 1.0;

        // calculate spread gains
        distances = speakerManager->getDistanceVector(currIdx);
        for (size_t chnl = 0; chnl < outputBuffers.size(); ++chnl) {
            if (chnl == channel) {
                continue;
            }
            // the distance row can be shorter than the channel count after `active set`
            gain = chnl < distances.size() ? _calculateGain(distances[chnl], spread[cnt]) : 0.0;
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
    SpeakerPair speakerPair;
    SpeakerPair channelPair;

    Param panRatio;

    Params distances;

    for (auto& buffer : outputBuffers) {
        buffer.assign(buffer.size(), 0.0f);
    }

    Samples& trjcty = inputBuffers[0];
    Samples& spread = inputBuffers[1];
    Samples& volume = inputBuffers[2];

    const size_t frames =
        std::min(trjcty.size(), outputBuffers.empty() ? size_t{0} : outputBuffers[0].size());

    for (size_t cnt = 0; cnt < frames; ++cnt) {
        speakerPair = speakerManager->getIndexesByTrajectory(trjcty[cnt]);

        // find, not operator[]: see _processTrigger
        auto first  = indexChannelLookup.find(speakerPair.first);
        auto second = indexChannelLookup.find(speakerPair.second);
        if (first == indexChannelLookup.end() || second == indexChannelLookup.end() ||
            first->second >= outputBuffers.size() || second->second >= outputBuffers.size()) {
            continue;
        }
        channelPair.first  = first->second;
        channelPair.second = second->second;

        if (speakerPair.first == speakerPair.second) {
            outputBuffers[channelPair.first][cnt] = volume[cnt];
        }
        else { // linear panning TODO: change to parameterized crossfade
            panRatio                               = speakerManager->getPanningRatio(trjcty[cnt]);
            outputBuffers[channelPair.first][cnt]  = volume[cnt] * (1 - panRatio);
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

    Param tmp = tan(theta * pi / 2.0);

    Param gain = isEqualTo0(tmp, VOLUME_THRESHOLD) ? 0.0 : 1.0 - x / tan(theta * pi / 2.0);

    // clip gain
    gain = gain < 0.0 ? 0 : gain;
    gain = gain > 1.0 ? 1 : gain;

    return gain;
}
