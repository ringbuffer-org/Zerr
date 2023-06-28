#include "utils.h"
#include "rolloff.h"

using namespace zerr;
using namespace feature;

const std::string Rolloff::name        = "";
const std::string Rolloff::category    = "";
const std::string Rolloff::description = "";

void Rolloff::initialize(t_systemConfigs sys_cfg){
    system_configs = sys_cfg;

    _reset_param();
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void Rolloff::extract(){

}

void Rolloff::reset(){

}

void Rolloff::fetch(t_featureInputs in){

}

t_featureValue Rolloff::send(){
    return y;
}

void Rolloff::_reset_param(){

}
