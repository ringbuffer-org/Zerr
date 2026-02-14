#include "flux.h"

using namespace zerr;
using namespace feature;

const std::string Flux::name     = "Spectral Flux";
const std::string Flux::category = "Frequency-Domain";
const std::string Flux::description =
    "Spectral flux is a measure used in digital signal processing that "
    "quantifies how quickly the power spectrum of a signal changes. It is "
    "often used in audio analysis for onset detection and other applications.";

void Flux::initialize(SystemConfigs sys_cfg)
{
    system_configs = sys_cfg;

    _reset_param();

    if (is_initialized() == false) {
        set_initialize_status(true);
    }
}

void Flux::extract()
{
    Param flux = 0.0;
    Param diff = 0.0;

    for (size_t i = 0; i < x.size(); ++i) {
        diff = x[i] - prv_x[i];
        flux += diff * diff;
    }

    crr_y = std::sqrt(flux);
}

void Flux::reset() { _reset_param(); }

void Flux::fetch(AudioInputs in)
{
    prv_x = x;
    x     = in.spec;

    prv_y = crr_y;
}

FeatureVals Flux::send()
{
    linear_interpolator.set_value(prv_y, crr_y, system_configs.block_size);

    for (size_t i = 0; i < system_configs.block_size; ++i) {
        y[i] = linear_interpolator.get_value();

        linear_interpolator.next_step();
    }

    return y;
}

void Flux::_reset_param()
{
    prv_x.resize(AUDIO_BUFFER_SIZE, 0.0f);
    x.resize(AUDIO_BUFFER_SIZE, 0.0f);

    prv_y = 0.0;
    crr_y = 0.0;
    y.resize(system_configs.block_size, 0.0f);
}