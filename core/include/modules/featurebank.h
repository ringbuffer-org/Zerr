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
    using CreateFunc = std::unique_ptr<FeatureExtractor> (*)(); /**< Function pointer type for creating
                                   FeatureExtractor objects  */
    typedef std::unique_ptr<FeatureExtractor> fe_ptr; /**< The unique_ptr of type virtual class FeatureExtractor  */
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
     * @brief Initialize the feature bank with selected features and system configuration
     * @param feature_names List of feature names to activate
     * @param system_configs System configuration parameters
     */
    void initialize(FeatureNames feature_names, SystemConfigs system_configs);
    /**
     * @brief Process an input audio block and extract all active features
     * @param in Input audio block to analyze
     * @return Map of feature names to extracted feature values
     */
    FeaturesVals perform(Block in);
    /**
     * @brief Reset the feature bank parameters and load a new set of features
     * @param feature_names New list of feature names to activate
     */
    void reset(FeatureNames feature_names);

 private:
    std::map<std::string, CreateFunc> registed_features; /**< Map between feature names and their constructor functions */

    std::vector<fe_ptr> activated_features; /**< Vector of pointers to activated feature objects */

    RingBuffer ring_buffer; /**< Ring buffer to hold previous audio samples for analysis */

    FrequencyTransformer freq_transformer; /**< FFT wrapper to perform frequency analysis
                                              on audio signal input */

    AudioInputs x; /**< Structure holding different types of feature inputs */

    FeaturesVals y; /**< Map containing extracted feature names and their values */

    int n_features; /**< Number of currently activated features */

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
