#pragma once

#include <stdlib.h>
#include <string>
#include <vector>

#include "featurebank.h"
#include "ringbuffer.h"
#include "types.h"
#include "utils.h"

/**
 * @class ZerrFeatures
 * @brief Main wrapper class that interfaces between Max/MSP and the core audio feature extraction functionality
 *
 * This class handles the initialization, audio processing, and cleanup of audio feature extraction operations.
 * It manages the data flow between Pure Data's audio system and the internal feature processing chain.
 */
class ZerrFeatures {
 public:
    int n_outlet; /**< Number of signal outlets based on enabled feature extractors */
    int n_inlet = 1; /**< Number of signal inlets for receiving audio input */
    /**
     * @brief Creates a new ZerrFeatures instance
     * @param sys_cnfg Pure Data system configuration containing sample rate and block size settings
     * @param ft_names List of audio features to extract from the input signal
     */
    ZerrFeatures(zerr::SystemConfigs sys_cnfg, zerr::t_featureNames ft_names)
        : input_buffer(n_inlet, std::vector<double>(sys_cnfg.block_size, 0.0f))
    {
        bank = new zerr::FeatureBank();

        systemConfigs.sample_rate = sys_cnfg.sample_rate;
        systemConfigs.block_size = sys_cnfg.block_size;

        for (int i = 0; i < ft_names.num; ++i) {
            featureNames.push_back(ft_names.names[i]);
        }
    };
    /**
     * @brief Initializes all internal components and prepares the object for processing
     * @return 1 if initialization was successful, 0 otherwise
     */
    int initialize()
    {
        try {
            bank->initialize(featureNames, systemConfigs);
        } catch (...) {
            // send bank initialize failed
            return 0;
        }

        n_outlet = featureNames.size();

        input_buffer.resize(n_inlet, std::vector<double>(systemConfigs.block_size, 0.0f));
        output_buffer.resize(n_outlet);

        in_ptr = (float**)malloc(n_inlet * sizeof(float**));
        out_ptr = (float**)malloc(n_outlet * sizeof(float**));

        return 1;
    };
    /**
     * @brief Main DSP callback function that processes audio buffers
     * @param ports Array of pointers to input/output audio buffers (shared memory between in/out)
     * @param n_vec The actual size of audio vectors to process (may be smaller than system block size)
     */
    void perform(float** ports, int n_vec)
    {
        in_ptr = (float**)&ports[0];
        out_ptr = (float**)&ports[n_inlet];

        for (int i = 0; i < n_inlet; i++) {
            for (int j = 0; j < n_vec; j++) {
                input_buffer[i][j] = in_ptr[i][j];
            }
        }

        output_buffer = bank->perform(input_buffer[0]);

        for (int i = 0; i < n_outlet; i++) {
            for (int j = 0; j < n_vec; j++) {
                out_ptr[i][j] = output_buffer[i][j];
            }
        }
    }
    /**
     * @brief Gets the total number of ports (inlets + outlets)
     * @return Total count of all audio ports
     */
    int get_port_count()
    {
        return n_inlet + n_outlet;
    }
    /**
     * @brief Destructor that cleans up and frees all allocated resources
     */
    ~ZerrFeatures()
    {
        delete bank;
    }

 private:
    zerr::SystemConfigs systemConfigs; /**< Pure Data system configuration settings */
    zerr::FeatureNames featureNames; /**< List of enabled audio feature extractors */

    zerr::Blocks input_buffer; /**< Buffer for storing incoming audio samples */
    zerr::FeaturesVals output_buffer; /**< Buffer for storing extracted feature values */

    float** in_ptr; /**< Array of pointers to Pure Data input signal vectors */
    float** out_ptr; /**< Array of pointers to Pure Data output signal vectors */

    std::vector<float*> in_tmp; /**< Temporary buffer for input signal processing */

    std::string zerr_cfg; /**< Path to configuration file */

    zerr::FeatureBank* bank; /**< Core component that implements the feature extraction algorithms */
};