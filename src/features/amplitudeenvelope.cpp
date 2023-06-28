#include "utils.h"
#include "amplitudeenvelope.h"

using namespace zerr;
using namespace feature;

const std::string AmplitudeEnvelope::name        = "";
const std::string AmplitudeEnvelope::category    = "";
const std::string AmplitudeEnvelope::description = "";

void AmplitudeEnvelope::initialize(t_systemConfigs sys_cfg){
    system_configs = sys_cfg;

    _reset_param();
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void AmplitudeEnvelope::extract(){

}

void AmplitudeEnvelope::reset(){

}

void AmplitudeEnvelope::fetch(t_featureInputs in){

}

t_featureValue AmplitudeEnvelope::send(){
    return y;
}

void AmplitudeEnvelope::_reset_param(){

}
