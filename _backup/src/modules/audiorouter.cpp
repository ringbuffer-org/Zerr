#include "audiorouter.h"
using namespace zerr;

typedef std::vector<input_vec> out_mat;

void AudioRouter::initialize(int num_audio, int num_cntrl){
    audio_x.resize(num_audio);
    cntrl_x.resize(num_cntrl);
    ys.resize(num_cntrl);
    for (auto& y : ys) {
        y.resize(num_audio);
    }
}

void AudioRouter::fetch(input_vec audio_in, cntrl_vec cntrl_in){
    audio_x = audio_in;
    cntrl_x = cntrl_in;

}

void AudioRouter::process(){
    for (int i = 0; i < cntrl_x.size(); ++i){
        for (int j = 0; j < audio_x.size(); ++j){
            ys[i][j] = cntrl_x[i]*audio_x[j];
        }
    }
}

out_mat AudioRouter::send(){
    return ys;
}
