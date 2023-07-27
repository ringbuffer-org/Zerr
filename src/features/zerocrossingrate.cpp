#include "utils.h"
#include "zerocrossingrate.h"

using namespace zerr;
using namespace feature;

const std::string ZeroCrossingRate::name        = "Zero crossing rate";
const std::string ZeroCrossingRate::category    = "Time-Domain";
const std::string ZeroCrossingRate::description = "The zero crossing rate (ZCR) is a measure of how frequently a signal changes its sign. It represents the rate at which the signal crosses the zero amplitude level over a given time period.";

void ZeroCrossingRate::initialize(t_systemConfigs sys_cfg){
    system_configs = sys_cfg;

    _reset_param();
    
    // zero_crossings     = 0;
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void ZeroCrossingRate::extract(){
    assert(is_initialized());

    int zero_crossings = 0;
    int x_size = x.size();

    for (int i = 1; i < x_size; ++i) {
        if ((x[i] >= 0 && x[i - 1] < 0) || (x[i] < 0 && x[i - 1] >= 0)) {
            ++zero_crossings;
        }
    }

    crr_y = static_cast<t_value>(zero_crossings) / (x_size - 1);
}

void ZeroCrossingRate::reset(){
    _reset_param();
}

void ZeroCrossingRate::fetch(t_featureInputs in){
    x = in.wave;
    prv_y = crr_y;
}

t_featureBuf ZeroCrossingRate::send(){
    linear_interpolator.set_value(prv_y, crr_y, system_configs.block_size);

    for (size_t i = 0; i < system_configs.block_size; ++i){
        y[i] = linear_interpolator.get_value();
        linear_interpolator.next_step();
    }

    return y;
}

void ZeroCrossingRate::_reset_param(){
    x.resize(AUDIO_BUFFER_SIZE, 0.0f);

    prv_y = 0.0;
    crr_y = 0.0;
    y.resize(system_configs.block_size, 0.0f);
}


