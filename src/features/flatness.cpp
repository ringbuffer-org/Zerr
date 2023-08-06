// #include "utils.h"
#include <numeric>
#include "flatness.h"

using namespace zerr;
using namespace feature;

const std::string Flatness::name        = "Spectral Flatness";
const std::string Flatness::category    = "Frequency-Domain";
const std::string Flatness::description = "Spectral flatness, also known as Wiener entropy, is a measure used in digital signal processing to characterize an audio spectrum. Spectral flatness is typically used to quantify how noise-like a signal is, as opposed to being tonal. A higher value of spectral flatness indicates a more noise-like signal, whereas a lower value indicates a more tonal signal.";

void Flatness::initialize(t_systemConfigs sys_cfg){
    system_configs = sys_cfg;

    _reset_param();
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void Flatness::extract(){
    // Calculate the sum of logarithms
    double logSum = 0.0;
    const double smallConstant = 1e-10;  // Small constant to avoid taking log of 0
    for (double sample : x) {
        logSum += std::log(sample + smallConstant);
    }

    // Calculate the geometric mean using exponentiation
    double geometricMean = std::exp(logSum / x.size());

    // Calculate the arithmetic mean
    double sum = std::accumulate(x.begin(), x.end(), 0.0);
    double arithmeticMean = sum / x.size();
    
    // Calculate the spectral flatness
    double flatness = 0.0;
    if (arithmeticMean != 0.0) {
        flatness = geometricMean / arithmeticMean;
    }

    crr_y = flatness;
}

void Flatness::reset(){
    _reset_param();
}

void Flatness::fetch(t_featureInputs in){
    x = in.spec;
    prv_y = crr_y;
}

t_featureBuf Flatness::send(){
    linear_interpolator.set_value(prv_y, crr_y, system_configs.block_size);

    for (size_t i = 0; i < system_configs.block_size; ++i){
        y[i] = linear_interpolator.get_value();
        linear_interpolator.next_step();
    }

    return y;
}

void Flatness::_reset_param(){
    x.resize(AUDIO_BUFFER_SIZE, 0.0f);

    prv_y = 0.0;
    crr_y = 0.0;
    y.resize(system_configs.block_size, 0.0f);
}
