#ifndef CREST_H
#define CREST_H

#include "configs.h"
#include "featureextractor.h"
#include "linearinterpolator.h"
#include "utils.h"

namespace zerr {
namespace feature {
/**
 * @brief Crest Factor algorithm - Calculates the ratio of peak amplitude to RMS value
 *
 * Category: Time-Domain
 *
 * Crest Factor is a parameter used in signal processing and audio engineering to describe
 * the characteristics of a waveform. It is defined as the ratio of the peak value of a
 * waveform to its RMS (Root Mean Square) value. Higher crest factors indicate more peaky
 * signals with greater dynamic range, while lower values suggest more consistent amplitude
 * levels. Also known as peak-to-average ratio (PAR).
 */
class CrestFactor : public FeatureExtractor {
  public:
    CrestFactor();

    /**
     * @brief Initialize the crest factor extractor with system configurations
     * @param sys_cfg System configuration parameters
     */
    void initialize(SystemConfigs sys_cfg);

    /**
     * @brief Extract the crest factor from the current audio frame
     */
    void extract();

    /**
     * @brief Reset the crest factor extractor state
     */
    void reset();

    /**
     * @brief Load new audio input data for processing
     * @param in Audio input data
     */
    void fetch(const AudioInputs& in);

    /**
     * @brief Get the calculated crest factor values
     * @return FeatureVals The extracted crest factor values
     */
    FeatureVals send();
    // FeatureVals perform(AudioInputs x);

  private:
    /**
     * @brief Reset internal parameters to initial state
     */
    void _reset_param();

    FeatureVal prv_y; ///< Previous crest factor value
    FeatureVal crr_y; ///< Current crest factor value

    LinearInterpolator linear_interpolator; ///< Interpolator for smoothing crest factor values
};

} // namespace feature
} // namespace zerr
#endif // CREST_H
