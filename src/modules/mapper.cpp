#include "mapper.h"
using namespace zerr;

void Mapper::initialize(std::string config_path){
    cold_down_time = 10;
    jump_cnt = 0;

    speaker_array.initialize(config_path);
    int n = speaker_array.get_n_speakers();
    _init_mapping(n);
    std::vector<int> tmp;
    tmp = speaker_array.get_random_speakers(0, 1);
    curr_idx = tmp[0];
    _update_mapping();
}


int Mapper::get_n_speaker(){
    return speaker_array.get_n_speakers();
}


void Mapper::_init_mapping(int n){
    mapping.clear(); //clean up the mapping if not empty
    mapping.push_back(1); // index 0: virtual point to store the overall vol
    for (int i = 0; i < n; ++i){
        mapping.push_back(0);
    }
}


void Mapper::_print_mapping(std::string note){
    std::cout<<note<<": ";
    for (t_value v: mapping){
        std::cout<<v<<"  ";
    }
    std::cout<<std::endl;
}


void Mapper::fetch(t_featureValueList in){
    x = in;
}


void Mapper::process(){

    if (jump_cnt==0){ //trigger received
        curr_idx = speaker_array.get_next_one_speaker(curr_idx, 1);
        jump_cnt = cold_down_time;
    }
    jump_cnt--;

    // curr_idx = speaker_array.get_next_one_speaker(curr_idx, 1);

    _update_mapping();
}


void Mapper::_update_mapping(){

    std::fill(mapping.begin(), mapping.end(), 0.0f);

    std::vector<int> c_spkrs = speaker_array.get_contiguous_speakers(curr_idx);
    
    mapping[0] = 1;        // index 0: virtual points
    mapping[curr_idx] = 1; // current activated index
    for (int i = 0; i < c_spkrs.size(); ++i){
        mapping[c_spkrs[i]] = 0.5;
    }
}


std::vector<t_value> Mapper::send(){

    // for (auto sample:mapping){
    //     std::cout<<sample<<" ";
    // }
    // std::cout<<std::endl;

    return mapping;
}