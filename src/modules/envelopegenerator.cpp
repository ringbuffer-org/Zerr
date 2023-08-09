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

    _set_index_channel_lookup(speakerManager->get_unmasked_indexs());

    currIdx = speakerManager->get_random_index();
    #ifdef TESTMODE
    logger->logDebug(formatString("EnvelopeGenerator::initialize currIdx %d", currIdx));
    #endif //TESTMODE

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
    return speakerManager->get_n_unmasked_speakers();
}


void EnvelopeGenerator::set_current_speaker(t_index newIdx){
    currIdx = newIdx;
}


void EnvelopeGenerator::manage_unmasked_indexs(std::string action, t_indexs idxs){
    speakerManager->manage_unmasked_indexs(action, idxs);
}


void EnvelopeGenerator::print_parameters(std::string name){
    if (name=="masks"){
        speakerManager->print_unmasked_indexs();
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
    t_value maxVal;

    for (size_t i = 0; i < inputBuffer[0].size(); ++i){
        currIdx = speakerManager->get_indexs_by_trigger(inputBuffer[0][i], currIdx, triggerMode);
        channel = indexChannelLookup[currIdx];
        outputBuffer[channel][i] = inputBuffer[2][i];

        distances = speakerManager->get_distance_vector(currIdx);
        for (size_t chnl = 0; chnl < outputBuffer.size(); ++chnl){
            if (chnl == channel) {continue;}
            outputBuffer[chnl][i] += _calculate_gain(distances[chnl], inputBuffer[1][i]) * inputBuffer[2][i]; // current activated index
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

    t_samples& volume =  inputBuffer[2];

    t_value panRatio;
    for (size_t i = 0; i < inputBuffer[0].size(); ++i){
        speakerPair = speakerManager->get_indexs_by_trajectory(inputBuffer[0][i]);
        channelPair.first  = indexChannelLookup[speakerPair.first];
        channelPair.second = indexChannelLookup[speakerPair.second];
        if (speakerPair.first == speakerPair.second) {
            outputBuffer[channelPair.first][i] = volume[i];
        } else { // linear panning TODO: change to crossfade 
            panRatio = speakerManager->get_panning_ratio(inputBuffer[0][i]);
            outputBuffer[channelPair.first][i]  = volume[i] * (1 - panRatio);
            outputBuffer[channelPair.second][i] = volume[i] * panRatio;
        }
    }
}


void EnvelopeGenerator::_set_index_channel_lookup(t_indexs indexs){
    for (size_t i = 0; i < indexs.size(); ++i){
        indexChannelLookup[indexs[i]] = i;
    }
}


t_blockOuts EnvelopeGenerator::send(){
    // TODO: tmp solution ask for advices
    for (size_t i = 0; i < outputBuffer.size(); ++i){
        outputBuffer[i] = applyMovingAverage(outputBuffer[i], 16);
    }
    return outputBuffer;
}


t_value EnvelopeGenerator::_calculate_normal_distribution(t_value x, t_value alpha) {
    t_value coefficient = 1.0 / (alpha * std::sqrt(2 * M_PI));
    t_value exponent = -0.5 * std::pow((x / alpha), 2);
    t_value value = coefficient * std::exp(exponent);
    if (value < VOLUME_THRESHOLD) {value = 0;}
    return value;
}


t_value EnvelopeGenerator::_calculate_gain(t_value x, t_value theta) {
    x = x * DISTANCE_SCALE;

    t_value tmp = tan(theta*PI/2.0);

    t_value gain = isEqualTo0(tmp, VOLUME_THRESHOLD)?0.0:1.0 - x/tan(theta*PI/2.0);

    return gain<0.0?0.0:gain;
}