/**
 * @file zerr_features.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief AudioFeatures Class Puredata Wrapper - Provides interface between Pure Data and the core audio feature extraction functionality
 * @date 2024-01-30
 *
 * @copyright Copyright (c) 2023-2024
 */

#pragma once

#include <string>
#include <vector>

#include "featurebank.h"
#include "ringbuffer.h"
#include "types.h"
#include "utils.h"

/**
 * @class ZerrFeatures
 * @brief Main wrapper class that interfaces between Pure Data and the core audio feature extraction functionality
 * 
 * This class handles the initialization, audio processing, and cleanup of audio feature extraction operations.
 * It manages the data flow between Pure Data's audio system and the internal feature processing chain.
 */
class ZerrFeatures {
  public:
    int n_outlet;    /**< Number of signal outlets based on enabled feature extractors */
    int n_inlet = 1; /**< Number of signal inlets for receiving audio input */
    /**
     * @brief Creates a new ZerrFeatures instance
     * @param sys_cnfg Pure Data system configuration containing sample rate and block size settings
     * @param ft_names List of audio features to extract from the input signal
     */
    ZerrFeatures(zerr::SystemConfigs sys_cnfg, zerr::t_featureNames ft_names);
    /**
     * @brief Initializes all internal components and prepares the object for processing
     * @return 1 if initialization was successful, 0 otherwise
     */
    int initialize();
    /**
     * @brief Main DSP callback function that processes audio buffers
     * @param ports Array of pointers to input/output audio buffers (shared memory between in/out)
     * @param n_vec The actual size of audio vectors to process (may be smaller than system block size)
     */
    void perform(float **ports, int n_vec);
    /**
     * @brief Gets the total number of ports (inlets + outlets)
     * @return Total count of all audio ports
     */
    int get_port_count();
    /**
     * @brief Destructor that cleans up and frees all allocated resources
     */
    ~ZerrFeatures();

  private:
    zerr::SystemConfigs systemConfigs; /**< Pure Data system configuration settings */
    zerr::FeatureNames featureNames;   /**< List of enabled audio feature extractors */

    zerr::Blocks input_buffer;         /**< Buffer for storing incoming audio samples */
    zerr::FeaturesVals output_buffer;  /**< Buffer for storing extracted feature values */

    float **in_ptr;   /**< Array of pointers to Pure Data input signal vectors */
    float **out_ptr;  /**< Array of pointers to Pure Data output signal vectors */

    std::vector<float *> in_tmp;  /**< Temporary buffer for input signal processing */

    std::string zerr_cfg;  /**< Path to configuration file */

    zerr::FeatureBank *bank;  /**< Core component that implements the feature extraction algorithms */
};
