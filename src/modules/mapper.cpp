#include "mapper.h"
using namespace zerr;

void Mapper::initialize(std::string config_path){
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
    // for testing
    std::cout<<note<<": ";
    for (float v: mapping){
        std::cout<<v<<"  ";
    }
    std::cout<<std::endl;
}

void Mapper::fetch(t_value in){
    //TODO: replace with ringbuffer insert
    prev_x = x; 
    x = in;
}

void Mapper::process(){
    if (x > 0.1 && prev_x < -0.1){ //trigger received
        curr_idx = speaker_array.get_next_one_speaker(curr_idx, 1);
    }

    // std::cout<<"Mapper::process  "<<curr_idx<<std::endl;

    _update_mapping();
}

void Mapper::_update_mapping(){
    //flush out mapping
    for (int i = 0; i < mapping.size(); ++i){
        mapping[i] = 0;
    }
    mapping[0] = 1;        // index 0: virtual points
    mapping[curr_idx] = 1; // current activated index
}

std::vector<float> Mapper::send(){

    for (auto sample:mapping){
        std::cout<<sample<<" ";
    }
    std::cout<<std::endl;

    return mapping;
}