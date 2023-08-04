#include "envelopegenerator.h"
using namespace zerr;

EnvelopeGenerator::EnvelopeGenerator(std::string spkr_cfg, std::string mode){
    this->speaker_config = spkr_cfg;
    this->mode = mode;

    speaker_manager = new SpeakerManager(this->speaker_config);
}

bool EnvelopeGenerator::initialize(){
    if (!speaker_manager->initialize()) return false;



    // int n = speaker_manager.get_n_unmasked_speakers();
    // _init_mapping(n);
    // std::vector<int> tmp;
    // tmp = speaker_manager.get_random_speakers(0, 1);
    // curr_idx = tmp[0];
    // _update_mapping();
}


// int EnvelopeGenerator::get_n_speaker(){
//     return speaker_manager.get_n_speakers();
// }


void EnvelopeGenerator::_init_mapping(int n){
    mapping.clear(); 
    mapping.push_back(1); // index 0: virtual point to store the overall vol
    for (int i = 0; i < n; ++i){
        mapping.push_back(0);
    }
}


void EnvelopeGenerator::fetch(t_featureValueList in){
    x = in;
}


void EnvelopeGenerator::process(){
    
    // if (trigger){ 
    //     curr_idx = speaker_manager.get_random_speakers(0, 1)[0];
    // }

    // _update_mapping();
}


void EnvelopeGenerator::_update_mapping(){

    // std::fill(mapping.begin(), mapping.end(), 0.0f);

    // std::vector<t_value> distances = speaker_manager.get_distance_vector(curr_idx);
    
    // mapping[0] = volume;        // index 0: overall valume

    // t_value max_val = _calculate_normal_distribution(0, width);

    // for (int i = 1; i < mapping.size(); ++i){
    //     mapping[i] = _calculate_normal_distribution(distances[i-1], 1)/max_val; // current activated index
    // }
}

t_blockOut EnvelopeGenerator::send(){
    // return mapping;
}


t_value EnvelopeGenerator::_calculate_normal_distribution(t_value x, t_value alpha) {
    t_value coefficient = 1.0 / (alpha * std::sqrt(2 * M_PI));
    t_value exponent = -0.5 * std::pow((x / alpha), 2);
    t_value value = coefficient * std::exp(exponent);
    if (value < VOLUME_THRESHOLD) {value = 0;}
    return value;
}