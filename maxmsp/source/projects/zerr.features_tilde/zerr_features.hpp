// #pragma once

// #include <stdlib.h>
// #include <string>
// #include <vector>

// #include "featurebank.h"
// #include "ringbuffer.h"
// #include "types.h"
// #include "utils.h"

// /**
//  * @class ZerrFeatures
//  * @brief Main wrapper class that interfaces between Max/MSP and the core audio feature extraction functionality
//  *
//  * This class handles the initialization, audio processing, and cleanup of audio feature extraction operations.
//  * It manages the data flow between Pure Data's audio system and the internal feature processing chain.
//  */
// class ZerrFeatures {
//  public:
//     int outputCount; /**< Number of signal outlets based on enabled feature extractors */
//     int inputCount = 1; /**< Number of signal inlets for receiving audio input */
//     /**
//      * @brief Creates a new ZerrFeatures instance
//      * @param sys_cnfg Pure Data system configuration containing sample rate and block size settings
//      * @param ft_names List of audio features to extract from the input signal
//      */
//     ZerrFeatures(zerr::SystemConfigs sys_cnfg, zerr::t_featureNames ft_names)
//         : input_buffer(inputCount, std::vector<double>(sys_cnfg.block_size, 0.0f))
//     {
//         bank = new zerr::FeatureBank();

//         systemConfigs.sample_rate = sys_cnfg.sample_rate;
//         systemConfigs.block_size = sys_cnfg.block_size;

//         for (int i = 0; i < ft_names.num; ++i) {
//             featureNames.push_back(ft_names.names[i]);
//         }
//     };
//     /**
//      * @brief Initializes all internal components and prepares the object for processing
//      * @return 1 if initialization was successful, 0 otherwise
//      */
//     int initialize()
//     {
//         try {
//             bank->initialize(featureNames, systemConfigs);
//         } catch (...) {
//             // send bank initialize failed
//             return 0;
//         }

//         outputCount = featureNames.size();

//         input_buffer.resize(inputCount, std::vector<double>(systemConfigs.block_size, 0.0f));
//         output_buffer.resize(outputCount);

//         in_ptr = (float**)malloc(inputCount * sizeof(float**));
//         out_ptr = (float**)malloc(outputCount * sizeof(float**));

//         return 1;
//     };
//     /**
//      * @brief Main DSP callback function that processes audio buffers
//      * @param ports Array of pointers to input/output audio buffers (shared memory between in/out)
//      * @param n_vec The actual size of audio vectors to process (may be smaller than system block size)
//      */
//     void perform(double** ins, long numins, double** outs, long numouts, long sampleframes)
//     {
//         // in_ptr = (float**)&ports[0];
//         // out_ptr = (float**)&ports[inputCount];

//         for (int i = 0; i < numins; i++) {
//             for (int j = 0; j < sampleframes; j++) {
//                 input_buffer[i][j] = ins[i][j];
//             }
//         }

//         output_buffer = bank->perform(input_buffer[0]);

//         for (int i = 0; i < numouts; i++) {
//             for (int j = 0; j < sampleframes; j++) {
//                 outs[i][j] = output_buffer[i][j];
//             }
//         }
//     }
//     /**
//      * @brief Gets the total number of ports (inlets + outlets)
//      * @return Total count of all audio ports
//      */
//     // int get_port_count()
//     // {
//     //     return inputCount + outputCount;
//     // }
//     /**
//      * @brief Destructor that cleans up and frees all allocated resources
//      */
//     ~ZerrFeatures()
//     {
//         delete bank;
//     }

//  private:
//     zerr::SystemConfigs systemConfigs; /**< Pure Data system configuration settings */
//     zerr::FeatureNames featureNames; /**< List of enabled audio feature extractors */

//     zerr::Blocks input_buffer; /**< Buffer for storing incoming audio samples */
//     zerr::FeaturesVals output_buffer; /**< Buffer for storing extracted feature values */

//     float** in_ptr; /**< Array of pointers to Pure Data input signal vectors */
//     float** out_ptr; /**< Array of pointers to Pure Data output signal vectors */

//     std::vector<float*> in_tmp; /**< Temporary buffer for input signal processing */

//     std::string zerr_cfg; /**< Path to configuration file */

//     zerr::FeatureBank* bank; /**< Core component that implements the feature extraction algorithms */
// };


#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>

#include "featurebank.h"
#include "ringbuffer.h"
#include "types.h"
#include "utils.h"

/**
 * @class ZerrFeatures
 * @brief Main wrapper class that interfaces between Max/MSP and the core audio feature extraction functionality
 *
 * This class handles the initialization, audio processing, and cleanup of audio feature extraction operations.
 * It manages the data flow between Max/MSP's audio system and the internal feature processing chain.
 */
class ZerrFeatures {
public:
    /**
     * @brief Creates a new ZerrFeatures instance
     * @param sys_config System configuration containing sample rate and block size settings
     * @param ft_names List of audio features to extract from the input signal
     */
    ZerrFeatures(const zerr::SystemConfigs& sys_config, const zerr::t_featureNames& ft_names)
        : systemConfigs{sys_config}
        , input_buffer(inputCount, std::vector<double>(sys_config.block_size, 0.0))
        , bank{std::make_unique<zerr::FeatureBank>()}
    {
        // Reserve space for efficiency
        featureNames.reserve(ft_names.num);
        
        for (int i = 0; i < ft_names.num; ++i) {
            featureNames.push_back(ft_names.names[i]);
        }
    }

    // Disable copying but allow moving
    ZerrFeatures(const ZerrFeatures&) = delete;
    ZerrFeatures& operator=(const ZerrFeatures&) = delete;
    ZerrFeatures(ZerrFeatures&&) = default;
    ZerrFeatures& operator=(ZerrFeatures&&) = default;

    /**
     * @brief Initializes all internal components and prepares the object for processing
     * @return true if initialization was successful, false otherwise
     * @throws std::runtime_error if memory allocation fails
     */
    bool initialize()
    {
        try {
            bank->initialize(featureNames, systemConfigs);
        } catch (const std::exception& e) {
            return false;
        }

        outputCount = static_cast<int>(featureNames.size());

        input_buffer.resize(inputCount, std::vector<double>(systemConfigs.block_size, 0.0));
        output_buffer.resize(outputCount);

        return true;
    }

    /**
     * @brief Main DSP callback function that processes audio buffers
     * @param ins Array of pointers to input audio buffers
     * @param numins Number of input channels
     * @param outs Array of pointers to output audio buffers
     * @param numouts Number of output channels
     * @param sampleframes Number of samples to process
     */
    void perform(double** ins, long numins, double** outs, long numouts, long sampleframes)
    {
        // Validate input parameters
        if (!ins || !outs || numins < inputCount || numouts < outputCount) {
            throw std::invalid_argument("Invalid buffer pointers or sizes in perform()");
        }

        // Use std::copy for better optimization possibilities
        for (int i = 0; i < numins; ++i) {
            std::copy_n(ins[i], sampleframes, input_buffer[i].begin());
        }

        // Process audio through the feature bank
        output_buffer = bank->perform(input_buffer[0]);

        // Copy output to destination buffers
        for (int i = 0; i < numouts; ++i) {
            std::copy_n(output_buffer[i].begin(), sampleframes, outs[i]);
        }
    }

    /**
     * @brief Gets the number of output channels
     * @return Number of output channels based on enabled feature extractors
     */
    [[nodiscard]] int getOutputCount() const noexcept { return outputCount; }

    /**
     * @brief Gets the number of input channels
     * @return Number of input channels (currently fixed at 1)
     */
    [[nodiscard]] constexpr int getInputCount() const noexcept { return inputCount; }

    /**
     * @brief Gets the total number of ports (inlets + outlets)
     * @return Total count of all audio ports
     */
    [[nodiscard]] int getPortCount() const noexcept { return inputCount + outputCount; }

    ~ZerrFeatures() = default;

private:
    static constexpr int inputCount = 1; /**< Number of signal inlets for receiving audio input */
    int outputCount = 0; /**< Number of signal outlets based on enabled feature extractors */
    
    zerr::SystemConfigs systemConfigs; /**< System configuration settings */
    zerr::FeatureNames featureNames; /**< List of enabled audio feature extractors */

    zerr::Blocks input_buffer; /**< Buffer for storing incoming audio samples */
    zerr::FeaturesVals output_buffer; /**< Buffer for storing extracted feature values */

    // std::vector<float*> in_ptrs; /**< Array of pointers to input signal vectors */
    // std::vector<float*> out_ptrs; /**< Array of pointers to output signal vectors */

    std::unique_ptr<zerr::FeatureBank> bank; /**< Core component that implements the feature extraction algorithms */
    std::string zerr_cfg; /**< Path to configuration file */
};