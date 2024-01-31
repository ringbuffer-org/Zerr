/**
 * @file zerr_features.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief AudioFeatures Class Puredata Wrapper
 * @date 2024-01-30
 * 
 * @copyright Copyright (c) 2023-2024
 */

#pragma once

#include <vector>
#include <string>

#include "utils.h"
#include "types.h"
#include "ringbuffer.h"

#include "featurebank.h"

class ZerrFeatures{
 public:
    int n_outlet;  /**< number of outlets, dynamic assigned */
    int n_inlet = 1;  /**< number of inlets, consant */
    /**
    * create a new ZerrFeatures instance 
    */
    ZerrFeatures(zerr::t_systemConfigs sys_cnfg, zerr::t_featureNames ft_names);
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
    zerr::t_systemConfigs systemConfigs;
    zerr::t_featureNameList featureNames;

    std::vector<std::vector<double>> input_buffer;  // TODO(Zeyu Yang): Fix datatypes
    zerr::t_featureValueList output_buffer;  // TODO(Zeyu Yang): Fix datatypes

    float **in_ptr;
    float **out_ptr;

    std::vector<float*> in_tmp;  // Vector of float pointers

    // config path
    std::string zerr_cfg;

    // module objects
    zerr::FeatureBank         *bank;
};





