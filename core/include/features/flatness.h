#ifndef FLATNESS_H
#define FLATNESS_H

#include "configs.h"
#include "featureextractor.h"
#include "linearinterpolator.h"
#include "utils.h"

namespace zerr {
namespace feature {
/**
 * @brief Spectral Flatness algorithm - Calculates the ratio of geometric to arithmetic mean of
 * spectrum
 *
 * The spectral flatness measures how noise-like or tone-like a sound is by comparing the
 * geometric mean to the arithmetic mean of the spectrum. Values closer to 1 indicate more
 * noise-like signals with energy spread across frequencies, while values closer to 0
 * suggest more tonal sounds with energy concentrated at specific frequencies.
 */
class Flatness : public FeatureExtractor {
  public:
    Flatness();

    /**
     * @brief Initialize the flatness extractor with system configurations
     * @param sys_cfg System configuration parameters
     */
    void initialize(SystemConfigs sys_cfg);

    /**
     * @brief Extract the spectral flatness from the current audio frame
     */
    void extract();

    /**
     * @brief Reset the flatness extractor state
     */
    void reset();

    /**
     * @brief Load new audio input data for processing
     * @param in Audio input data
     */
    void fetch(const AudioInputs& in);

    /**
     * @brief Get the calculated flatness values
     * @return FeatureVals The extracted flatness values
     */
    FeatureVals send();
    // FeatureVals perform(AudioInputs x);

  private:
    /**
     * @brief Reset internal parameters to initial state
     */
    void _reset_param();

    FeatureVal prv_y; ///< Previous flatness value
    FeatureVal crr_y; ///< Current flatness value

    LinearInterpolator linear_interpolator; ///< Interpolator for smoothing flatness values
};

} // namespace feature
} // namespace zerr
#endif // FLATNESS_H
