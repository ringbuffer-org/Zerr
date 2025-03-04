#include "frequencytransformer.h"
using namespace zerr;

FrequencyTransformer::FrequencyTransformer(int L) {
    frame_size = L;
    fft_size = (L / 2 + 1);

    fft_in = new double[frame_size];
    for (int i = 0; i < frame_size; i++) {
        fft_in[i] = 0;
    }

    fft_out = new fftw_complex[fft_size];

    power_spec.resize(fft_size);

    p_fft = fftw_plan_dft_r2c_1d(frame_size, fft_in, fft_out, FFTW_ESTIMATE);
    p_ifft = fftw_plan_dft_c2r_1d(frame_size, fft_out, fft_in, FFTW_ESTIMATE);
}

void FrequencyTransformer::fft() { fftw_execute(p_fft); }

void FrequencyTransformer::ifft() { fftw_execute(p_ifft); }

void FrequencyTransformer::power_spectrum() {
    for (int i = 0; i < fft_size; i++) {
        power_spec[i] =
            (1.0 / (2.0 * (float)fft_size)) *
            (fft_out[i][0] * fft_out[i][0] + fft_out[i][1] * fft_out[i][1]);
    }
}

void FrequencyTransformer::windowing() {
    for (int i = 0; i < frame_size; i++)
        fft_in[i] = fft_in[i] * get_hann_sample(i, frame_size);
}

double* FrequencyTransformer::fft_input() { return fft_in; }

fftw_complex* FrequencyTransformer::fft_output() { return fft_out; }

AudioBuffer FrequencyTransformer::get_power_spectrum() { return power_spec; }