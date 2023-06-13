#include "audiorouter.h"
using namespace zerr;

typedef std::vector<t_blockIn> out_mat;

void AudioRouter::initialize(int num_audio, int num_speaker){
    audio_x.resize(num_audio);
    cntrl_x.resize(num_speaker+1);
    ys.resize(num_speaker);
    for (auto& y : ys) {
        y.resize(num_audio);
    }
}

void AudioRouter::fetch(t_blockIn audio_in, cntrl_vec cntrl_in){
    audio_x.clear();
    cntrl_x.clear();
    audio_x = audio_in;
    cntrl_x = cntrl_in;
}

void AudioRouter::process(){
    for (int i = 0; i < cntrl_x.size()-1; ++i){
        for (int j = 0; j < audio_x.size(); ++j){
            ys[i][j] = cntrl_x[0] * cntrl_x[i+1] * audio_x[j];
        }
    }
}

out_mat AudioRouter::send(){
    return ys;
}
