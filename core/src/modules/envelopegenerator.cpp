/**
 * @file envelopegenerator.cpp 
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Envelope Generator Class Implementation
 * @date 2024-01-31
 * 
 * @copyright Copyright (c) 2023-2024
 */
#include "envelopegenerator.h"
// using namespace zerr;
using zerr::EnvelopeGenerator;
using zerr::Blocks;
using zerr::Param;

EnvelopeGenerator::EnvelopeGenerator(SystemConfigs systemCfgs, std::string spkrCfgFile, std::string selectionMode){
    this->systemCfgs    = systemCfgs;
    this->spkrCfgFile   = spkrCfgFile;
    this->selectionMode = selectionMode;

    speakerManager = new SpeakerManager(this->spkrCfgFile);

    logger = new Logger();
    #ifdef TESTMODE
    logger->setLogLevel(LogLevel::INFO);
    #endif //TESTMODE
}


bool EnvelopeGenerator::initialize(){
    if (!speakerManager->initialize()) return false;

    int numOutlet = get_n_speaker();

    inputBuffer.resize(numInlet,   Samples(systemCfgs.block_size, 0.0f));
    outputBuffer.resize(numOutlet, Samples(systemCfgs.block_size, 0.0f));

    _set_index_channel_lookup(speakerManager->getActiveSpeakerIndexs());

    currIdx = speakerManager->get_random_index(); // TODO(Zeyu Yang): gives option to set init
    #ifdef TESTMODE
    logger->logDebug(formatString("EnvelopeGenerator::initialize currIdx %d", currIdx));
    #endif  // TESTMODE

    if (selectionMode!="trigger" && selectionMode!="trajectory"){ 
        logger->logError("EnvelopeGenerator::initialize Unknown selection mode: " + selectionMode);
        return false;
    }

    triggerMode = 0; // Random

    return true;
}



Blocks EnvelopeGenerator::perform(Blocks in) {
    // fetch
    inputBuffer = in;

    // process
    if (selectionMode=="trigger"){_process_trigger();}
    if (selectionMode=="trajectory"){_process_trajectory();}

    // send
    // TODO: make this optional
    // for (size_t i = 0; i < outputBuffer.size(); ++i){
    //     outputBuffer[i] = applyMovingAverage(outputBuffer[i], 16);
    // }
    return outputBuffer;
}

int EnvelopeGenerator::get_n_speaker(){
    return speakerManager->getNumActiveSpeakers();
}


void EnvelopeGenerator::set_current_speaker(Index newIdx){
    currIdx = newIdx;
}


void EnvelopeGenerator::setActiveSpeakerIndexs(std::string action, Indexes idxs){
    speakerManager->setActiveSpeakers(action, idxs);
}


void EnvelopeGenerator::setTrajectoryVector(Indexes idxs){
    speakerManager->setTrajectoryVector(idxs);
}

void EnvelopeGenerator::setTopoMatrix(std::string action, Indexes idxs){
    speakerManager->setTopoMatrix(action, idxs);
}


void EnvelopeGenerator::printParameters(){
    // if (name=="masks"){
    //     speakerManager->printActiveSpeakerIndexs();
    // }else{
    //     logger->logError("EnvelopeGenerator::print_parameters unknown parameter " + name);
    // }
    speakerManager->printParameters();
}

//TODO: Clean this up
void EnvelopeGenerator::_process_trigger(){
    int channel;
    for (auto& buffer : outputBuffer) {
        buffer.assign(buffer.size(), 0.0f);
    }

    std::vector<Param> distances;

    Samples& triggr =  inputBuffer[0];
    Samples& spread =  inputBuffer[1];
    Samples& volume =  inputBuffer[2];

    for (size_t i = 0; i < inputBuffer[0].size(); ++i){
        currIdx = speakerManager->get_indexs_by_trigger(inputBuffer[0][i], currIdx, triggerMode);
        channel = indexChannelLookup[currIdx];
        outputBuffer[channel][i] = volume[i];

        distances = speakerManager->get_distance_vector(currIdx);
        for (size_t chnl = 0; chnl < outputBuffer.size(); ++chnl){
            if (chnl == channel) {continue;}
            outputBuffer[chnl][i] = calculateGain(distances[chnl], spread[i]) * volume[i]; // current activated index
        }
    }
}

//TODO: Clean this up
void EnvelopeGenerator::_process_trajectory(){
    for (auto& buffer : outputBuffer) {
        buffer.assign(buffer.size(), 0.0f);
    }
    t_pair speakerPair;
    t_pair channelPair;

    Samples& trjcty =  inputBuffer[0];
    Samples& spread =  inputBuffer[1];
    Samples& volume =  inputBuffer[2];

    Param panRatio;
    std::vector<Param> distances;
    for (size_t i = 0; i < trjcty.size(); ++i){
        speakerPair = speakerManager->get_indexs_by_trajectory(trjcty[i]);
        channelPair.first  = indexChannelLookup[speakerPair.first];
        channelPair.second = indexChannelLookup[speakerPair.second];
        if (speakerPair.first == speakerPair.second) {
            outputBuffer[channelPair.first][i] = volume[i];
        } else { // linear panning TODO: change to parameterized crossfade 
            panRatio = speakerManager->get_panning_ratio(trjcty[i]);
            outputBuffer[channelPair.first][i]  = volume[i] * (1 - panRatio);
            outputBuffer[channelPair.second][i] = volume[i] * panRatio;
        }
        // process the spread of first speaker
        distances = speakerManager->get_distance_vector(speakerPair.first);
        for (size_t chnl = 0; chnl < outputBuffer.size(); ++chnl){
            if (chnl == channelPair.first) {continue;}
            outputBuffer[chnl][i] += calculateGain(distances[chnl], spread[i]) * volume[i]; //* (1 - panRatio); // current activated index
        }
        // process the spread of second speaker
        distances = speakerManager->get_distance_vector(speakerPair.second);
        for (size_t chnl = 0; chnl < outputBuffer.size(); ++chnl){
            if (chnl == channelPair.second) {continue;}
            outputBuffer[chnl][i] += calculateGain(distances[chnl], spread[i]) * volume[i]; //* panRatio; // current activated index
        }
    }
}


void EnvelopeGenerator::_set_index_channel_lookup(Indexes indexs){
    for (size_t i = 0; i < indexs.size(); ++i){
        indexChannelLookup[indexs[i]] = i;
    }
}


Param EnvelopeGenerator::calculateGain(Param x, Param theta) {
    x = x * DISTANCE_SCALE;

    // clip the theta
    theta = theta<0.0?0:theta;
    theta = theta>1.0?1:theta;

    Param tmp = tan(theta*PI/2.0);

    Param gain = isEqualTo0(tmp, VOLUME_THRESHOLD)?0.0:1.0 - x/tan(theta*PI/2.0);

    // clip gain
    gain = gain<0.0?0:gain;
    gain = gain>1.0?1:gain;

    return gain;
}