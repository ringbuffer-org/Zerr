#include "utils.h"
#include "zerocrossings.h"

using namespace zerr;
using namespace feature;

const std::string ZeroCrossings::name        = "";
const std::string ZeroCrossings::category    = "";
const std::string ZeroCrossings::description = "";

void ZeroCrossings::initialize(t_systemConfigs sys_cfg){
    system_configs = sys_cfg;

    _reset_param();
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void ZeroCrossings::extract(){

}

void ZeroCrossings::reset(){

}

void ZeroCrossings::fetch(t_featureInputs in){

}

t_featureValue ZeroCrossings::send(){
    return y;
}

void ZeroCrossings::_reset_param(){

}
