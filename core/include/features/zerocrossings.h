#ifndef ZEROCROSSINGS_H
#define ZEROCROSSINGS_H

#include "configs.h"
#include "featureextractor.h"
#include "utils.h"

namespace zerr {
namespace feature {
/**
 * @brief Zero Crossings algorithm - Counts the number of times a signal crosses the zero level
 *
 * Category: Sample-Level
 *
 * Zero crossing is used to describe the point at which a signal changes its sign from
 * positive to negative or from negative to positive. Unlike zero crossing rate, this
 * provides the absolute count rather than a rate normalized by frame length. Higher counts
 * indicate more high frequency content or noise-like signals, while lower counts suggest
 * more low frequency or tonal content. This can help characterize signal periodicity and
 * frequency content.
 */
class ZeroCrossings : public FeatureExtractor {
  public:
    ZeroCrossings();

    /**
     * @brief Initialize the zero crossings extractor with system configurations
     * @param sys_cfg System configuration parameters
     */
    void initialize(SystemConfigs sys_cfg);

    /**
     * @brief Extract the zero crossings count from the current audio frame
     */
    void extract();

    /**
     * @brief Reset the zero crossings extractor state
     */
    void reset();

    /**
     * @brief Load new audio input data for processing
     * @param in Audio input data
     */
    void fetch(const AudioInputs& in);

    /**
     * @brief Get the calculated zero crossings values
     * @return FeatureVals The extracted zero crossings counts
     */
    FeatureVals send();
    // FeatureVals perform(AudioInputs x);

  private:
    /**
     * @brief Reset internal parameters to initial state
     */
    void _reset_param();

    /**
     * @brief Check if there is a zero crossing between two samples
     * @param first First sample value
     * @param second Second sample value
     * @return bool True if there is a zero crossing, false otherwise
     */
    bool _isZeroCrossing(Sample first, Sample second);

    Sample last_sample; ///< Last sample from previous frame for zero crossing detection
};

} // namespace feature
} // namespace zerr
#endif // ZEROCROSSINGS_H
