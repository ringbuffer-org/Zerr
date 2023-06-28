#include "utils.h"
#include "crest.h"

using namespace zerr;
using namespace feature;

const std::string Crest::name        = "";
const std::string Crest::category    = "";
const std::string Crest::description = "";

void Crest::initialize(t_systemConfigs sys_cfg){
    system_configs = sys_cfg;

    _reset_param();
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void Crest::extract(){

}

void Crest::reset(){

}

void Crest::fetch(t_featureInputs in){

}

t_featureValue Crest::send(){
    return y;
}

void Crest::_reset_param(){

}
