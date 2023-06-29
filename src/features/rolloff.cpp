#include "utils.h"
#include "rolloff.h"

using namespace zerr;
using namespace feature;

const std::string Rolloff::name        = "Spectral Rolloff";
const std::string Rolloff::category    = "Frequency-Domain";
const std::string Rolloff::description = "The spectral rolloff is a measure used in signal processing to determine the frequency below which a specified percentage of the total spectral energy lies. It is often used to distinguish between harmonic and non-harmonic content in an audio signal.";

void Rolloff::initialize(t_systemConfigs sys_cfg){
    system_configs = sys_cfg;
    freq_max = static_cast<double>(system_configs.sample_rate) / 2.0;

    _reset_param();
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void Rolloff::extract(){

    // Calculate the total energy in the spectrum
    double totalEnergy = std::accumulate(x.begin(), x.end(), 0.0);
    
    // Calculate the energy threshold for the rolloff
    double rolloffThreshold = totalEnergy * rolloffPercent;
    
    // Find the rolloff frequency
    double sumEnergy = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        sumEnergy += x[i];
        if (sumEnergy >= rolloffThreshold) {
            // Calculate the frequency corresponding to the bin index
            crr_y = (double)i * freq_max / (double)x.size();
            return;
        }
    }
    // If we reach this point, the rolloff frequency is the Nyquist frequency
    crr_y = freq_max;
}

void Rolloff::reset(){
    _reset_param();
}

void Rolloff::fetch(t_featureInputs in){
    x = in.spec;
    prv_y = crr_y;
}

t_featureBuf Rolloff::send(){
    linear_interpolator.set_value(prv_y, crr_y, system_configs.block_size);

    for (size_t i = 0; i < system_configs.block_size; ++i){
        y[i] = linear_interpolator.get_value();
        linear_interpolator.next_step();
    }

    return y;
}

void Rolloff::_reset_param(){
    x.resize(AUDIO_BUFFER_SIZE, 0.0f);

    prv_y = 0.0;
    crr_y = 0.0;
    y.resize(system_configs.block_size, 0.0f);
}
