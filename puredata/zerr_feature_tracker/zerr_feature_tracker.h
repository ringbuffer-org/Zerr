/**
 * @file zerr_feature_tracker.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Zerr Feature Class Puredata Wrapper
 * @version 0.3
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 */

#pragma once

// #include "zerr_config.h"

// dependencies
#include "utils.h"
#include "types.h"
#include "ringbuffer.h"

// modules
#include "featurebank.h"

class ZerrFeatureTracker{
public:
    int n_outlet;
    int n_inlet=1;
    /**
    * create a new zerr instance 
    */
    ZerrFeatureTracker(zerr::t_systemConfigs sys_cnfg, zerr::t_featureNames ft_names);
    /**
    * initialize all zerr modules
    */
    int initialize();
    /**
    * callback function, process a block of samples
    */
    void perform(float **ports, int n_vec);
    /**
    * return the total number of inlet plus outlet
    * 
    */
    int get_port_count();
    /**
    * free a zerr instance
    */
    ~ZerrFeatureTracker();

private:
    //basic config
    zerr::t_systemConfigs system_configs;
    zerr::t_featureNameList feature_names;

    std::vector<std::vector<double>> input_buffer;

    zerr::t_featureValueList output_buffer;
    float **in_ptr;
    float **out_ptr;

    std::vector<float*> in_tmp;  // Vector of float pointers

    // config path
    std::string zerr_cfg;

    //module objects
    zerr::FeatureBank         *bank;
};





