#include "onset.h"
#include "utils.h"
using namespace zerr;
using namespace feature;

const std::string Onset::name        = "Onset";
const std::string Onset::category    = "spectrum";
const std::string Onset::description = "The spectral centroid is a measure used in digital signal processing to characterise a spectrum.";

void Onset::initialize(t_systemConfigs sys_cfg){

    #ifdef TESTMODE
    system_configs = sys_cfg;
    std::cout<<"Onset::initialize sample_rate | "<< system_configs.sample_rate<<std::endl;
    #endif
    
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void Onset::extract(){
    std::cout<<"Onset: extract"<<std::endl;
}

void Onset::reset(){
    std::cout<<"Onset::reset"<<std::endl;
}

void Onset::fetch(t_featureInputs in){
    x.clear();
    x = in.wave;
}

t_featureValue Onset::send(){
    return y;
}
