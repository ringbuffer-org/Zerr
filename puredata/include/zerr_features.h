/**
 * @file zerr_features.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief AudioFeatures Class Puredata Wrapper
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

class ZerrFeatures {
  public:
    int n_outlet;    /**< number of outlets, dynamic assigned */
    int n_inlet = 1; /**< number of inlets, consant */
    /**
     * create a new ZerrFeatures instance
     */
    ZerrFeatures(zerr::SystemConfigs sys_cnfg, zerr::t_featureNames ft_names);
    /**
     * initialize ZerrFeatures modules
     */
    int initialize();
    /**
     * callback function, process a block of samples
     */
    void perform(float **ports, int n_vec);
    /**
     * return the total number of inlet plus outlet
     */
    int get_port_count();
    /**
     * free a ZerrFeatures instance
     */
    ~ZerrFeatures();

  private:
    // basic config
    zerr::SystemConfigs systemConfigs;
    zerr::FeatureNames featureNames;

    zerr::Blocks input_buffer;         // TODO(Zeyu Yang): Fix datatypes
    zerr::FeaturesVals output_buffer;  // TODO(Zeyu Yang): Fix datatypes

    float **in_ptr;
    float **out_ptr;

    std::vector<float *> in_tmp;  // Vector of float pointers

    // config path
    std::string zerr_cfg;

    // module objects
    zerr::FeatureBank *bank;
};
