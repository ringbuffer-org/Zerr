#include "audiorouter.h"
using namespace zerr;

typedef std::vector<std::vector<double>> out_mat;

void AudioRouter::initialize(int num_audio, int num_cntrl){
    // std::cout<<"initialize!"<<std::endl;
    audio_x.resize(num_audio);
    cntrl_x.resize(num_cntrl);
    // audio_x.clear();
    // for (int i = 0; i < num_audio; ++i){
    //     audio_x.push_back(0);
    // }
    // cntrl_x.clear();
    // for (int i = 0; i < num_cntrl; ++i){
    //     cntrl_x.push_back(0);
    // }
    // ys.clear();
    ys.resize(num_cntrl);
    for (auto& y : ys) {
        y.resize(num_audio);
    }
}

void AudioRouter::fetch(audio_vec audio_in, cntrl_vec cntrl_in){
    // std::cout<<"fetch!"<<std::endl;

    audio_x = audio_in;
    cntrl_x = cntrl_in;

    //test
    // std::cout<<"audio_x: "<<std::endl;
    // for (auto x:audio_x){
    //     std::cout<<x<<" ";
    // }
    // std::cout<<std::endl;
    // std::cout<<"cntrl_x: "<<std::endl;
    // for (auto x:cntrl_x){
    //     std::cout<<x<<" ";
    // }
    // std::cout<<std::endl;
}

void AudioRouter::process(){
    // std::cout<<"process!"<<std::endl;
    for (int i = 0; i < cntrl_x.size(); ++i){
        for (int j = 0; j < audio_x.size(); ++j){
            ys[i][j] = cntrl_x[i]*audio_x[j];
        }
    }
}

out_mat AudioRouter::send(){
    // std::cout<<"send!"<<std::endl;
    return ys;
}
