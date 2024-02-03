#include "envelopegenerator.h"
using namespace zerr;


EnvelopeGenerator::EnvelopeGenerator(t_systemConfigs systemCfgs, std::string spkrCfgFile, std::string selectionMode){
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

    inputBuffer.resize(numInlet,   t_samples(systemCfgs.block_size, 0.0f));
    outputBuffer.resize(numOutlet, t_samples(systemCfgs.block_size, 0.0f));

    _set_index_channel_lookup(speakerManager->getActiveSpeakerIndexs());

    currIdx = speakerManager->get_random_index();
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


void EnvelopeGenerator::fetch(t_blockIns in){
    inputBuffer = in;
}


int EnvelopeGenerator::get_n_speaker(){
    return speakerManager->getNumActiveSpeakers();
}


void EnvelopeGenerator::set_current_speaker(t_index newIdx){
    currIdx = newIdx;
}


void EnvelopeGenerator::manage_unmasked_indexs(std::string action, t_indexs idxs){
    speakerManager->manageActiveSpeakerIndexs(action, idxs);
}


void EnvelopeGenerator::setTrajectoryVector(t_indexs idxs){
    speakerManager->setTrajectoryVector(idxs);
}


void EnvelopeGenerator::print_parameters(std::string name){
    if (name=="masks"){
        speakerManager->printActiveSpeakerIndexs();
    }else{
        logger->logError("EnvelopeGenerator::print_parameters unknown parameter " + name);
    }
}


void EnvelopeGenerator::process(){
    if (selectionMode=="trigger"){_process_trigger();}
    if (selectionMode=="trajectory"){_process_trajectory();}
}

//TODO: Clean this up
void EnvelopeGenerator::_process_trigger(){
    int channel;
    for (auto& buffer : outputBuffer) {
        buffer.assign(buffer.size(), 0.0f);
    }

    std::vector<t_value> distances;

    t_samples& triggr =  inputBuffer[0];
    t_samples& spread =  inputBuffer[1];
    t_samples& volume =  inputBuffer[2];

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

    t_samples& trjcty =  inputBuffer[0];
    t_samples& spread =  inputBuffer[1];
    t_samples& volume =  inputBuffer[2];

    t_value panRatio;
    std::vector<t_value> distances;
    for (size_t i = 0; i < trjcty.size(); ++i){
        speakerPair = speakerManager->get_indexs_by_trajectory(trjcty[i]);
        channelPair.first  = indexChannelLookup[speakerPair.first];
        channelPair.second = indexChannelLookup[speakerPair.second];
        if (speakerPair.first == speakerPair.second) {
            outputBuffer[channelPair.first][i] = volume[i];
        } else { // linear panning TODO: change to crossfade 
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


void EnvelopeGenerator::_set_index_channel_lookup(t_indexs indexs){
    for (size_t i = 0; i < indexs.size(); ++i){
        indexChannelLookup[indexs[i]] = i;
    }
}


t_blockOuts EnvelopeGenerator::send(){
    // TODO: make this optional
    // for (size_t i = 0; i < outputBuffer.size(); ++i){
    //     outputBuffer[i] = applyMovingAverage(outputBuffer[i], 16);
    // }
    
    return outputBuffer;
}


t_value EnvelopeGenerator::calculateGain(t_value x, t_value theta) {
    x = x * DISTANCE_SCALE;

    theta = theta<0.0?0:theta;
    theta = theta>1.0?1:theta;

    t_value tmp = tan(theta*PI/2.0);

    t_value gain = isEqualTo0(tmp, VOLUME_THRESHOLD)?0.0:1.0 - x/tan(theta*PI/2.0);

    gain = gain<0.0?0:gain;
    gain = gain>1.0?1:gain;

    return gain;
}