#include "utils.h"
#include "zerocrossings.h"

using namespace zerr;
using namespace feature;

const std::string ZeroCrossings::name        = "ZeroCrossings";
const std::string ZeroCrossings::category    = "Sample-Level";
const std::string ZeroCrossings::description = "Zero crossing is used to describe the point at which a signal changes its sign from positive to negative or from negative to positive.";

void ZeroCrossings::initialize(t_systemConfigs sys_cfg){
    system_configs = sys_cfg;

    _reset_param();
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void ZeroCrossings::extract(){

    if (_isZeroCrossing(last_sample,x[0])) y[0] = 1;

    for (size_t i = 1; i < x.size(); ++i){
        if (_isZeroCrossing(x[i-1],x[i])) y[i] = 1;
    }

    last_sample = x[x.size() - 1];
}

void ZeroCrossings::reset(){
    _reset_param();
}

void ZeroCrossings::fetch(t_featureInputs in){
    x = in.blck;
    y.resize(x.size(), 0.0f);
}

t_featureBuf ZeroCrossings::send(){
    return y;
}

void ZeroCrossings::_reset_param(){
    x.resize(system_configs.block_size, 0.0f);
    y.resize(system_configs.block_size, 0.0f);
    last_sample = 0.0;
}

bool ZeroCrossings::_isZeroCrossing(t_sample first, t_sample second){
    return ((first >= 0 && second < 0) || (first < 0 && second >= 0));
}
