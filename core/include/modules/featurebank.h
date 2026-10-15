#ifndef FEATUREBANK_H
#define FEATUREBANK_H

#include <iostream>
#include <map>
#include <memory>

#include "audio_features.h"
#include "configs.h"
#include "featureextractor.h"
#include "frequencytransformer.h"
#include "ringbuffer.h"
#include "utils.h"

namespace zerr {
/**
 * @brief FeatureBank class manages and coordinates multiple audio feature extractors
 *
 * FeatureBank is used to organize the behavior of all feature extractors uniformly.
 * It takes original audio block input and performs preprocessing like buffering and FFT,
 * then distributes the results to all activated feature extraction algorithms.
 */
class FeatureBank {
  public:
    using CreateFunc = std::unique_ptr<FeatureExtractor> (*)(); /**< Function pointer type for
                                   creating FeatureExtractor objects  */
    using fe_ptr = std::unique_ptr<FeatureExtractor>; /**< The unique_ptr of type virtual class
                                                         FeatureExtractor  */
    /**
     * @brief FeatureBank Constructor
     *
     * Registers all available features and initializes the input buffer
     */
    FeatureBank();
    /**
     * @brief Print detailed information about a specific feature
     * @param name The name of the feature to get information about
     */
    void print_info(std::string name);
    /**
     * @brief Print the names of all supported features
     */
    void print_all_features();
    /**
     * @brief Print the names of currently active features
     */
    void print_active_features();
    /**
     * @brief Activate a set of features and prepare them for a signal shape
     * @param feature_names List of feature names to activate (replaces any previous set)
     * @param system_configs Initial sample rate and block size; see prepare()
     * @throws std::runtime_error when a name is not registered or the shape is invalid
     *
     * Idempotent: calling it again replaces the active set instead of appending to it.
     */
    void initialize(FeatureNames feature_names, SystemConfigs system_configs);
    /**
     * @brief Adopt a sample rate and block size (see SystemConfigs for the contract)
     * @return false, leaving the bank untouched, when the block size is 0 or larger than
     *         the AUDIO_BUFFER_SIZE analysis window, or the sample rate is 0
     *
     * Returns at once when the shape is unchanged. Otherwise every active extractor is
     * re-initialized for the new shape, which restarts its smoothing state.
     */
    bool prepare(SystemConfigs system_configs);
    /**
     * @brief Process an input audio block and extract all active features
     * @param in Input audio block to analyze; samples beyond the prepared block size are ignored
     * @return One block of values per active feature, each of the prepared block size
     */
    FeaturesVals perform(Samples in);
    /**
     * @brief The sample rate and block size the bank is currently prepared for
     */
    [[nodiscard]] SystemConfigs getSystemConfigs() const noexcept { return system_configs; }
    /**
     * @brief The short names every FeatureBank can activate, in registration order
     *
     * Static so a host can list or validate names without building a bank (the
     * constructor allocates FFT plans). Wrappers print this in their error messages
     * instead of carrying their own copy of the table.
     */
    static FeatureNames availableFeatures();

  private:
    std::map<std::string, CreateFunc>
        registed_features; /**< Map between feature names and their constructor functions */

    std::vector<fe_ptr> activated_features; /**< Vector of pointers to activated feature objects */

    FeatureNames active_feature_names; /**< Names of currently activated features */

    RingBuffer ring_buffer; /**< Ring buffer to hold previous audio samples for analysis */

    FrequencyTransformer freq_transformer; /**< FFT wrapper to perform frequency analysis
                                              on audio signal input */

    AudioInputs x; /**< Structure holding different types of feature inputs */

    FeaturesVals y; /**< Map containing extracted feature names and their values */

    int n_features; /**< Number of currently activated features */

    SystemConfigs system_configs{0, 0}; /**< Shape the active extractors are prepared for */

    /**
     * @brief Register all available feature extractors to the FeatureBank
     *
     * This should only run inside FeatureBank constructor and run only once
     */
    void _regist_all();
    /**
     * @brief Register a single feature extractor to the bank
     * @param className Name of the feature class to register
     * @param createFunc Constructor function for the feature
     *
     * This should only run inside regist_all function
     */
    void _regist(const std::string& className, CreateFunc createFunc);
    /**
     * @brief Create a new instance of a feature extractor by name
     * @param className Name of the feature class to instantiate
     * @return Unique pointer to the created feature extractor
     *
     * This should only run inside initialize function
     */
    std::unique_ptr<FeatureExtractor> _create(const std::string& className);
};

} // namespace zerr
#endif // FEATUREBANK_H
