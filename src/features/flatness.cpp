#include "utils.h"
#include "flatness.h"

using namespace zerr;
using namespace feature;

const std::string Flatness::name        = "";
const std::string Flatness::category    = "";
const std::string Flatness::description = "";

void Flatness::initialize(t_systemConfigs sys_cfg){
    system_configs = sys_cfg;

    _reset_param();
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void Flatness::extract(){

}

void Flatness::reset(){

}

void Flatness::fetch(t_featureInputs in){

}

t_featureValue Flatness::send(){
    return y;
}

void Flatness::_reset_param(){

}
