/**
 * @file frequencytransformer.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief FFT class dependent on fftw3 library for frequency domain analysis
 * @date 2023-06-10
 *
 * @copyright Copyright (c) 2023-2024
 */
#ifndef FREQUENCYTRANSFORMER_H
#define FREQUENCYTRANSFORMER_H

#include <fftw3.h>

#include "types.h"
#include "utils.h"
namespace zerr {

class FrequencyTransformer {
  public:
    /**
     * @brief Construct a new Frequency Transformer object
     * @param L The frame size for FFT analysis
     */
    FrequencyTransformer(int L);
    /**
     * @brief Perform Fast Fourier Transform
     * 
     * Run FFT on data in fft_in buffer and save the result to fft_out buffer.
     * The input should be real-valued time domain signal.
     */
    void fft();
    /**
     * @brief Perform Inverse Fast Fourier Transform
     * 
     * Run IFFT on fft_out buffer and save the result to fft_in buffer.
     * Converts frequency domain data back to time domain.
     */
    void ifft();
    /**
     * @brief Calculate power spectrum
     * 
     * Computes the power spectrum using the complex FFT data in fft_out buffer.
     * Power spectrum represents the signal's energy at each frequency.
     */
    void power_spectrum();
    /**
     * @brief Get pointer to FFT input buffer
     * @return double* Pointer to the real-valued input buffer
     */
    double* fft_input();
    /**
     * @brief Get pointer to FFT output buffer
     * @return fftw_complex* Pointer to the complex-valued FFT output buffer
     */
    fftw_complex* fft_output();
    /**
     * @brief Get the computed power spectrum
     * @return AudioBuffer Buffer containing the power spectrum values
     */
    AudioBuffer get_power_spectrum();

    /**
     * @brief Apply window function to input data
     * 
     * Applies windowing to reduce spectral leakage in FFT analysis
     */
    void windowing();

    /**
     * @brief Get the frame size used for analysis
     * @return int The frame size in samples
     */
    int get_frame_size() { return frame_size; }

  private:
    int frame_size;      ///< Size of the analysis frame in samples
    int fft_size;        ///< Size of the FFT (typically frame_size/2 + 1)

    AudioBuffer power_spec;  ///< Buffer to store power spectrum results

    double* fft_in;         ///< Input buffer for FFT
    fftw_complex* fft_out;  ///< Output buffer for FFT results

    fftw_plan p_fft, p_ifft;  ///< FFTW plan objects for forward and inverse transforms
};

}  // namespace zerr
#endif  // FREQUENCYTRANSFORMER_H