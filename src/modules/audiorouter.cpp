#include "audiorouter.h"
using namespace zerr;

typedef std::vector<t_blockIn> out_mat;

void AudioRouter::initialize(int num_audio, int num_speaker){
    audio_x.resize(num_audio, 0.0f);
    cntrl_x.resize(num_speaker, 0.0f);

    y.resize(num_speaker);
    for (auto& vec : y) {
        vec.resize(num_audio);
    }

    linear_interpolator.initialize(num_speaker);
}

void AudioRouter::fetch(t_blockIn audio_in, t_volumes cntrl_in){

    cntrl_x_prev = cntrl_x;

    main_volume = cntrl_in[0];

    audio_x = audio_in;

    cntrl_x.assign(cntrl_in.begin() + 1, cntrl_in.end());

    linear_interpolator.set_values(cntrl_x_prev, cntrl_x, audio_x.size());
}

void AudioRouter::process(){
    t_volumes tmp;
    
    for (int j = 0; j < audio_x.size(); ++j){

        tmp = linear_interpolator.get_value();

        for (int i = 0; i < cntrl_x.size(); ++i){
            y[i][j] = main_volume * tmp[i] * audio_x[j];
        }

        linear_interpolator.next_step();
    }
}

out_mat AudioRouter::send(){
    return y;
}
