#ifndef FEATUREBANK_H
#define FEATUREBANK_H

#include <iostream>
#include <map>
#include <memory>

#include "utils.h"
#include "configs.h"
#include "ringbuffer.h"
#include "frequencytransformer.h"

#include "audio_features.h"
#include "featureextractor.h"

namespace zerr {
/**
* FeatureBank is used to organise the behaviour of all feature extractors uniformly
* It takes original audio block input and does preprocess like Buffering, FFT
* then distributes the results to all activated feature extraction algorithms
*/
class FeatureBank {
public:   
    using CreateFunc = std::unique_ptr<FeatureExtractor> (*)(); /**< Function pointer type for creating FeatureExtractor objects  */
    typedef std::unique_ptr<FeatureExtractor> fe_ptr; /**< The unique_ptr of type virtual class FeatureExtractor  */
    /**
    * FeatureBank Constructor
    * Regist all available features and initialize the input buffer
    */
    FeatureBank();
    /**
    * Print the detail info of selected feature
    */
    void print_info(std::string name);
    /**
    * Print the name of all supported features
    */
    void print_all_features();
    /**
    * Print the name of active features
    */
    void print_active_features();
    /**
    * Print the name of active features
    */
    void initialize(FeatureNames feature_names, SystemConfigs system_configs);
    /**
    * fetch: dsp callback function
    * fetch audio block and store in the buffer if needed
    * Warning: n_vec could be smaller than system block_size. Error occurs in this scenario
    */
    void fetch(Block in);
    /**
    * process: dsp callback function
    * process the audio sample in the input buffer
    * and save process results to the output buffer
    */
    void process();
    /**
    * send: dsp callback function
    * send results in output buffer to other module
    */
    FeaturesVals send();
    /**
    * Reset the featurebank parameters and load new features
    */
    void reset(FeatureNames feature_names);

private:

    std::map<std::string, CreateFunc> registed_features; /**< The map between all feature names and feature constructors*/

    std::vector<fe_ptr> activated_features; /**< The pointer to activated feature objects*/

    RingBuffer ring_buffer; /**< basic ring buffer to hold previous audio samples */

    FrequencyTransformer freq_transformer; /**< fftw3 warper to perform fft on audio signal input*/

    AudioInputs x;    /**< the structure to hold different type of feature inputs */

    FeaturesVals y; /**< The map between all feature names and feature constructors */

    int n_features; /**< The number of activated features */

    /**
    * Regist all feature extractors to the FeatureBank
    * This should only run inside FeatureBank constructor and run only once
    */
    void _regist_all();
    /**
    * Regist a feature extractor to the bank
    * This should only run inside regist_all function
    */
    void _regist(const std::string& className, CreateFunc createFunc);
    /**
    * Retrieve the constructor of feature extractor selected by name.
    * This should only run inside initialize function
    */
    std::unique_ptr<FeatureExtractor> _create(const std::string& className);
};

} //namespace zerr
#endif // FEATUREBANK_H

