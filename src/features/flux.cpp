#include "flux.h"

using namespace zerr;
using namespace feature;

const std::string Flux::name        = "Spectral Flux";
const std::string Flux::category    = "Frequency-Domain";
const std::string Flux::description = "Spectral flux is a measure used in digital signal processing that quantifies how quickly the power spectrum of a signal changes. It is often used in audio analysis for onset detection and other applications.";

void Flux::initialize(t_systemConfigs sys_cfg){
    system_configs = sys_cfg;

    _reset_param();
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void Flux::extract(){

    t_value flux = 0.0;
    t_value diff = 0;

    for (size_t i = 0; i < x.size(); ++i) {
            diff  = x[i] - pre_x[i];
            flux += diff * diff;
    }

    y.original = std::sqrt(flux);
    y.normalized = y.original;
}

void Flux::reset(){
    #ifdef TESTMODE
    std::cout<<"Flux::reset"<<std::endl;
    #endif //TESTMODE
}

void Flux::fetch(t_featureInputs in){
    pre_x = x;
    x = in.spec;
}

t_featureValue Flux::send(){
    return y;
}

void Flux::_reset_param(){
    pre_x.resize(AUDIO_BUFFER_SIZE, 0.0f);
    x.resize(AUDIO_BUFFER_SIZE, 0.0f);
}