/**
 * @file frequencytransformer.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com) 
 * @brief FFT class dependent on fftw3 library
 * @date 2023-06-10
 * 
 * @copyright Copyright (c) 2023-2024
 */
#ifndef FREQUENCYTRANSFORMER_H
#define FREQUENCYTRANSFORMER_H

#include "utils.h"
#include <fftw3.h>
#include "types.h"
namespace zerr {

class FrequencyTransformer{
public:
    /**
    * FrequencyTransformer Constructor
    */
    FrequencyTransformer(int L);
    /**
    * run fft on data in fft_in buffer
    * and save the result to fft_out buffer
    */
    void fft();
    /**
    * run ifft on fft_out buffer
    * and save the result to fft_in buffer
    */
    void ifft();
    /**
    * calculate power spectrum using the data in fft_out buffer
    */
    void power_spectrum();
    /**
    * get the fft result
    */
    double* fft_input();
    /**
    * get the ifft result
    */
    fftw_complex* fft_output();
    t_specBuf get_power_spectrum();

    void windowing();

    int get_frame_size(){return frame_size;}

private:
    int frame_size;
    int fft_size;

    t_specBuf power_spec;

    double       *fft_in;
    fftw_complex *fft_out;

    fftw_plan p_fft, p_ifft;
};

} // namespace zerr
#endif // FREQUENCYTRANSFORMER_H