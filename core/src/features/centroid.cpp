#include "centroid.h"
#include "utils.h"

using namespace zerr;
using namespace feature;

const std::string Centroid::name     = "Spectral Centroid";
const std::string Centroid::category = "Frequency-Domain";
const std::string Centroid::description =
    "The spectral centroid is a measure used in digital signal processing to "
    "characterise a spectrum.";

void Centroid::initialize(SystemConfigs sys_cfg)
{
    system_configs = sys_cfg;
    freq_max       = static_cast<double>(system_configs.sample_rate) / 2.0;

    _reset_param();

    if (is_initialized() == false) {
        set_initialize_statue(true);
    }
}

void Centroid::extract()
{
    double centroid       = 0.0;
    double totalMagnitude = 0.0;

    int fft_size = x.size();
    for (int i = 0; i < fft_size; ++i) {
        centroid += (i * freq_max / fft_size) * x[i];
        totalMagnitude += x[i];
    }

    if (totalMagnitude > 0.0) {
        centroid /= totalMagnitude;
    }

    crr_y = centroid;
}

void Centroid::reset() { _reset_param(); }

void Centroid::fetch(AudioInputs in)
{
    x     = in.spec;
    prv_y = crr_y;
}

FeatureVals Centroid::send()
{
    linear_interpolator.set_value(prv_y, crr_y, system_configs.block_size);

    for (size_t i = 0; i < system_configs.block_size; ++i) {
        y[i] = linear_interpolator.get_value();
        linear_interpolator.next_step();
    }

    return y;
}

void Centroid::_reset_param()
{
    x.resize(AUDIO_BUFFER_SIZE, 0.0f);

    prv_y = 0.0;
    crr_y = 0.0;
    y.resize(system_configs.block_size, 0.0f);
}
