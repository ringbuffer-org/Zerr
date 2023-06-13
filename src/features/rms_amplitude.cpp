#include "utils.h"
#include "rms_amplitude.h"

using namespace zerr;
using namespace feature;

const std::string RMSAmplitude::name        = "Root-Mean-Squre Amplitude";
const std::string RMSAmplitude::category    = "Time-Domain";
const std::string RMSAmplitude::description = "the RMS is defined as the square root of the mean over time of the square of the vertical distance of the graph from the rest state";

void RMSAmplitude::initialize(t_systemConfigs sys_cfg){
    _reset_param();
    system_configs = sys_cfg;
    
    #ifdef TESTMODE
    // std::cout<<"RMSAmplitude::initialize sample_rate | "<< system_configs.sample_rate<<std::endl;
    #endif

    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void RMSAmplitude::extract(){
    _reset_param();
    for (int i = 0; i < wave_size; ++i) {
        square_sum += x[i]*x[i];
    }
    square_sum = square_sum / wave_size;
    square_root = std::sqrt(square_sum);

    #ifdef TESTMODE
    // std::cout<<"RMSAmplitude: "<<wave_size<<" "<<square_sum<<" "<<square_root<<std::endl;
    #endif //TESTMODE

    y.original   = square_root;
    y.normalized = square_root;
}

void RMSAmplitude::reset(){
    #ifdef TESTMODE
    std::cout<<"RMSAmplitude::reset"<<std::endl;
    #endif //TESTMODE
}

void RMSAmplitude::fetch(t_featureInputs in){
    x.clear();
    x = in.wave;
    wave_size = x.size();
}

t_featureValue RMSAmplitude::send(){
    return y;
}

void RMSAmplitude::_reset_param(){
    square_sum = 0;
    square_root = 0;
}

