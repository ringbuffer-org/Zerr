#include "utils.h"
#include "centroid.h"

using namespace zerr;
using namespace feature;

const std::string Centroid::name        = "Centroid";
const std::string Centroid::category    = "Frequency-Domain";
const std::string Centroid::description = "The spectral centroid is a measure used in digital signal processing to characterise a spectrum.";

void Centroid::initialize(t_systemConfigs sys_cfg){
    #ifdef TESTMODE
    system_configs = sys_cfg;
    freq_max = static_cast<double>(system_configs.sample_rate) / 2.0 ;
    std::cout<<"Centroid::initialize sample_rate | "<< system_configs.sample_rate<<std::endl;
    std::cout<<"Centroid::initialize freq_max | "<< freq_max<<std::endl;
    #endif

    _reset_param();
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void Centroid::extract(){
    //reset the parameters
    _reset_param();
    int fft_size = x.size();
    for (int i = 0; i < fft_size; ++i) {
        centroid += (i * freq_max / fft_size) * x[i];
        totalMagnitude += x[i];
    }

    if (totalMagnitude > 0.0) {
        centroid /= totalMagnitude;
    }

    y.original = centroid; // 
}

void Centroid::reset(){
    std::cout<<"Centroid::reset"<<std::endl;
}

void Centroid::fetch(t_featureInputs in){
    x.clear();
    x = in.spec;
}

t_featureValue Centroid::send(){
    return y;
}

void Centroid::_reset_param(){
    centroid = 0.0;
    totalMagnitude = 0.0;
}
