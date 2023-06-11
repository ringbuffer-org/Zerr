#include "utils.h"
#include "zerocrossingrate.h"

using namespace zerr;
using namespace feature;

const std::string ZeroCrossingRate::name        = "Zero Crossing Rate";
const std::string ZeroCrossingRate::category    = "Time-Domain";
const std::string ZeroCrossingRate::description = "The zero crossing rate (ZCR) is a measure of how frequently a signal changes its sign. It represents the rate at which the signal crosses the zero amplitude level over a given time period.";

void ZeroCrossingRate::initialize(t_systemConfigs sys_cfg){

    #ifdef TESTMODE
    system_configs = sys_cfg;
    std::cout<<"ZeroCrossingRate::initialize sample_rate | "<< system_configs.sample_rate<<std::endl;
    #endif
    
    zero_crossings     = 0;
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void ZeroCrossingRate::extract(){
    assert(is_initialized());
    zero_crossings = 0;
    for (size_t i = 1; i < x.size(); ++i) {
        if ((x[i] >= 0 && x[i - 1] < 0) || (x[i] < 0 && x[i - 1] >= 0)) {
            ++zero_crossings;
        }
    }

    y.original = static_cast<t_value>(zero_crossings) / (x.size() - 1);
    y.normalized = y.original;  // There is no different between two outputs
}

void ZeroCrossingRate::reset(){

    #ifdef TESTMODE
    std::cout<<"ZeroCrossingRate::reset"<<std::endl;
    #endif

    zero_crossings = 0;
}

void ZeroCrossingRate::fetch(t_featureInputs in){
    x.clear();
    x = in.wave;
}

t_featureValue ZeroCrossingRate::send(){
    return y;
}


