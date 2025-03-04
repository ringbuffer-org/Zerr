#include "crestfactor.h"
#include "utils.h"

using namespace zerr;
using namespace feature;

const std::string CrestFactor::name = "Crest Factor";
const std::string CrestFactor::category = "Time-Domain";
const std::string CrestFactor::description =
    "Crest Factor is a parameter used in signal processing and audio "
    "engineering to describe the characteristics of a waveform. It is defined "
    "as the ratio of the peak value of a waveform to its RMS (Root Mean "
    "Square) value. ";

void CrestFactor::initialize(SystemConfigs sys_cfg) {
    system_configs = sys_cfg;

    _reset_param();

    if (is_initialized() == false) {
        set_initialize_statue(true);
    }
}

void CrestFactor::extract() {
    double square_sum = 0.0;
    double square_root = 0.0;
    double peak_max = 0.0;
    double peak_tmp = 0.0;
    int x_size = x.size();

    for (int i = 0; i < x_size; ++i) {
        square_sum += x[i] * x[i];

        peak_tmp = abs(x[i]);
        peak_max = peak_tmp > peak_max ? peak_tmp : peak_max;
    }

    square_sum = square_sum / x_size;
    square_root = std::sqrt(square_sum);

    crr_y = peak_max / square_root;
}

void CrestFactor::reset() { _reset_param(); }

void CrestFactor::fetch(AudioInputs in) {
    x = in.wave;
    prv_y = crr_y;
}

FeatureVals CrestFactor::send() {
    linear_interpolator.set_value(prv_y, crr_y, system_configs.block_size);

    for (size_t i = 0; i < system_configs.block_size; ++i) {
        y[i] = linear_interpolator.get_value();
        linear_interpolator.next_step();
    }

    return y;
}

void CrestFactor::_reset_param() {
    x.resize(AUDIO_BUFFER_SIZE, 0.0f);

    prv_y = 0.0;
    crr_y = 0.0;
    y.resize(system_configs.block_size, 0.0f);
}
