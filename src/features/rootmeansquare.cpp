#include "utils.h"
#include "rootmeansquare.h"

using namespace zerr;
using namespace feature;

const std::string RootMeanSquare::name        = "Root-Mean-Squre Amplitude";
const std::string RootMeanSquare::category    = "Time-Domain";
const std::string RootMeanSquare::description = "the RMS is defined as the square root of the mean over time of the square of the vertical distance of the graph from the rest state";

void RootMeanSquare::initialize(t_systemConfigs sys_cfg){
    _reset_param();
    system_configs = sys_cfg;
    
    #ifdef TESTMODE
    // std::cout<<"RootMeanSquare::initialize sample_rate | "<< system_configs.sample_rate<<std::endl;
    #endif

    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void RootMeanSquare::extract(){
    _reset_param();
    for (int i = 0; i < wave_size; ++i) {
        square_sum += x[i]*x[i];
    }
    square_sum = square_sum / wave_size;
    square_root = std::sqrt(square_sum);

    #ifdef TESTMODE
    // std::cout<<"RootMeanSquare: "<<wave_size<<" "<<square_sum<<" "<<square_root<<std::endl;
    #endif //TESTMODE

    y.original   = square_root;
    y.normalized = square_root;
}

void RootMeanSquare::reset(){
    #ifdef TESTMODE
    std::cout<<"RootMeanSquare::reset"<<std::endl;
    #endif //TESTMODE
}

void RootMeanSquare::fetch(t_featureInputs in){
    x.clear();
    x = in.wave;
    wave_size = x.size();
}

t_featureValue RootMeanSquare::send(){
    return y;
}

void RootMeanSquare::_reset_param(){
    square_sum = 0;
    square_root = 0;
}

