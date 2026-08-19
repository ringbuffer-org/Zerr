#ifndef CENTROID_H
#define CENTROID_H

#include "configs.h"
#include "featureextractor.h"
#include "linearinterpolator.h"
#include "utils.h"

namespace zerr {
namespace feature {
/**
 * @brief Spectral Centroid algorithm - Calculates the center of mass of the spectrum
 *
 * Category: Frequency-Domain
 *
 * The spectral centroid is a measure used in digital signal processing to characterise
 * a spectrum. It represents the "brightness" of a sound by indicating where the center
 * of mass of the spectrum is located. Higher centroid values generally correspond to
 * brighter sounds with more high frequency content.
 */
class Centroid : public FeatureExtractor {
  public:
    Centroid();

    /**
     * @brief Initialize the centroid extractor with system configurations
     * @param sys_cfg System configuration parameters
     */
    void initialize(SystemConfigs sys_cfg);

    /**
     * @brief Extract the spectral centroid from the current audio frame
     */
    void extract();

    /**
     * @brief Reset the centroid extractor state
     */
    void reset();

    /**
     * @brief Load new audio input data for processing
     * @param in Audio input data
     */
    void fetch(const AudioInputs& in);

    /**
     * @brief Get the calculated centroid values
     * @return FeatureVals The extracted centroid values
     */
    FeatureVals send();
    // FeatureVals perform(AudioInputs x);

  private:
    /**
     * @brief Reset internal parameters to initial state
     */
    void _reset_param();

    FeatureVal prv_y; ///< Previous centroid value
    FeatureVal crr_y; ///< Current centroid value

    double freq_max; ///< Maximum frequency considered for centroid calculation

    LinearInterpolator linear_interpolator; ///< Interpolator for smoothing centroid values
};

} // namespace feature
} // namespace zerr
#endif // CENTROID_H