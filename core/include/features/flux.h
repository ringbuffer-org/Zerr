#ifndef FLUX_H
#define FLUX_H

#include "configs.h"
#include "featureextractor.h"
#include "linearinterpolator.h"
#include "utils.h"

namespace zerr {
namespace feature {
/**
 * @brief Spectral Flux algorithm - Calculates frame-to-frame changes in the spectrum
 *
 * Category: Frequency-Domain
 *
 * Spectral flux is a measure used in digital signal processing that quantifies how quickly
 * the power spectrum of a signal changes. It is calculated as the difference between
 * consecutive spectral frames. Higher flux values indicate more rapid spectral changes,
 * while lower values suggest more steady spectral content. It is often used in audio
 * analysis for onset detection and other applications.
 */
class Flux : public FeatureExtractor {
  public:
    Flux();

    /**
     * @brief Initialize the flux extractor with system configurations
     * @param sys_cfg System configuration parameters
     */
    void initialize(SystemConfigs sys_cfg);

    /**
     * @brief Extract the spectral flux from the current audio frame
     */
    void extract();

    /**
     * @brief Reset the flux extractor state
     */
    void reset();

    /**
     * @brief Load new audio input data for processing
     * @param in Audio input data
     */
    void fetch(const AudioInputs& in);

    /**
     * @brief Get the calculated flux values
     * @return FeatureVals The extracted flux values
     */
    FeatureVals send();
    // FeatureVals perform(AudioInputs x);

  private:
    /**
     * @brief Reset internal parameters to initial state
     */
    void _reset_param();

    Samples prv_x;    ///< Previous spectral frame
    FeatureVal prv_y; ///< Previous flux value
    FeatureVal crr_y; ///< Current flux value

    LinearInterpolator linear_interpolator; ///< Interpolator for smoothing flux values
};

} // namespace feature
} // namespace zerr
#endif // FLUX_H