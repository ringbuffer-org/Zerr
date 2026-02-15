/**
 * @file rolloff.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Implementation of spectral rolloff algorithm
 * @date 2023-05-28
 *
 * @copyright Copyright (c) 2023
 */

#ifndef ROLLOFF_H
#define ROLLOFF_H

#include "configs.h"
#include "featureextractor.h"
#include "linearinterpolator.h"
#include "utils.h"

namespace zerr {
namespace feature {
/**
 * @brief Spectral Rolloff algorithm - Calculates frequency threshold containing N% of spectral
 * energy
 *
 * The spectral rolloff is a measure used in signal processing to determine the
 * frequency below which a specified percentage of the total spectral energy
 * lies. It is often used to distinguish between harmonic and non-harmonic
 * content in an audio signal. Higher rolloff frequencies indicate more high-frequency
 * content, while lower values suggest more energy concentrated in lower frequencies.
 */
class Rolloff : public FeatureExtractor {
  public:
    Rolloff();

    /**
     * @brief Initialize the rolloff extractor with system configurations
     * @param sys_cfg System configuration parameters
     */
    void initialize(SystemConfigs sys_cfg);

    /**
     * @brief Extract the spectral rolloff from the current audio frame
     */
    void extract();

    /**
     * @brief Reset the rolloff extractor state
     */
    void reset();

    /**
     * @brief Load new audio input data for processing
     * @param in Audio input data
     */
    void fetch(const AudioInputs& in);

    /**
     * @brief Get the calculated rolloff values
     * @return FeatureVals The extracted rolloff values
     */
    FeatureVals send();
    // FeatureVals perform(AudioInputs x);

  private:
    /**
     * @brief Reset internal parameters to initial state
     */
    void _reset_param();

    FeatureVal prv_y; ///< Previous rolloff value
    FeatureVal crr_y; ///< Current rolloff value

    double freq_max; ///< Maximum frequency considered for rolloff calculation
    double rolloffPercent =
        DEFAULT_ROLLOFF_PERCENT; ///< Percentage threshold for spectral energy accumulation

    LinearInterpolator linear_interpolator; ///< Interpolator for smoothing rolloff values
};

} // namespace feature
} // namespace zerr
#endif // ROLLOFF_H
