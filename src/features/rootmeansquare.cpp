#include "utils.h"
#include "rootmeansquare.h"

using namespace zerr;
using namespace feature;

const std::string RootMeanSquare::name        = "Root-Mean-Squre Amplitude";
const std::string RootMeanSquare::category    = "Time-Domain";
const std::string RootMeanSquare::description = "the RMS is defined as the square root of the mean over time of the square of the vertical distance of the graph from the rest state";

void RootMeanSquare::initialize(t_systemConfigs sys_cfg){
    system_configs = sys_cfg;

    _reset_param();
    
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void RootMeanSquare::extract(){
    double square_sum  = 0;
    double square_root = 0;
    int    x_size      = x.size();

    for (size_t i = 0; i < x_size; ++i) {
        square_sum += x[i]*x[i];
    }
    square_sum = square_sum / x_size;
    square_root = std::sqrt(square_sum);

    crr_y   = square_root;
}

void RootMeanSquare::reset(){
    _reset_param();
}

void RootMeanSquare::fetch(t_featureInputs in){
    x = in.wave;
    prv_y = crr_y;
}

t_featureBuf RootMeanSquare::send(){
    linear_interpolator.set_value(prv_y, crr_y, system_configs.block_size);

    for (size_t i = 0; i < system_configs.block_size; ++i){
        y[i] = linear_interpolator.get_value();
        linear_interpolator.next_step();
    }

    return y;
}

void RootMeanSquare::_reset_param(){
    x.resize(AUDIO_BUFFER_SIZE, 0.0f);

    prv_y = 0.0;
    crr_y = 0.0;
    y.resize(system_configs.block_size, 0.0f);
}

