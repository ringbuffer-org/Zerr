#ifndef FLUX_H
#define FLUX_H

#include "utils.h"
#include "configs.h"
#include "featureextractor.h"
#include "linearinterpolator.h"

namespace zerr {
namespace feature{
/**
 * @brief Spectral Flux algorithm - Calculates frame-to-frame changes in the spectrum
 *
 * The spectral flux measures how quickly the spectrum is changing between successive frames
 * by calculating the difference between consecutive spectral frames. Higher flux values
 * indicate more rapid spectral changes, while lower values suggest more steady spectral
 * content. This can help detect note onsets and transitions in audio signals.
 */
class Flux : public FeatureExtractor {
public:
    static const std::string name;        ///< Name identifier for this feature
    static const std::string category;    ///< Category this feature belongs to
    static const std::string description; ///< Description of what this feature measures

    /**
     * @brief Get the name identifier of this feature
     * @return std::string The feature name
     */
    std::string get_name(){return name;}

    /**
     * @brief Get the category this feature belongs to
     * @return std::string The feature category
     */
    std::string get_category(){return category;}

    /**
     * @brief Get the description of what this feature measures
     * @return std::string The feature description
     */
    std::string get_description(){return description;}

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
    void fetch(AudioInputs in);

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

    Samples      prv_x; ///< Previous spectral frame
    FeatureVal prv_y;   ///< Previous flux value
    FeatureVal crr_y;   ///< Current flux value

    LinearInterpolator linear_interpolator; ///< Interpolator for smoothing flux values
};

} //namespace feature
} //namespace zerr
#endif // FLUX_H