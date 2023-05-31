/**
 * @file zerr.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Zerr Class Puredata Wrapper
 * @version 0.3
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 */

#pragma once

#include <string.h>

#include "zerr_config.h"

// dependencies
#include "utils.h"
#include "types.h"

// modules
#include "audiorouter.h"
#include "featurebank.h"
#include "mapper.h"
#include "trajectorygenerator.h"

#include "m_pd.h" // for testing

// #ifdef __cplusplus
// extern "C" {
// #endif

/**
 * @brief Main GOAT class
 */
// typedef struct zerr_pd{
//     int n_outlet;
//     sys_config cfg; /**< Puredata config */
// } zerr_pd;

// namespace zerr{

class Zerr{
public:
    // n_outlet = 8;
    int n_outlet;
    /**
    * setup configs files and 
    */
    Zerr(SystemConfig sys_cnfg, std::string spkrCfgFile);
    /**
    * initialize all zerr modules
    */
    void initialize();
    /**
    * callback function
    */
    void perform(float *in, float *out, int n);
    /**
    * deconstructe zerr
    */
    ~Zerr();

private:
    //basic config
    int sample_rate; 
    int block_size;
    std::vector<std::vector<float>> output_buffer;

    // config path
    std::string zerr_cfg;
    std::string spkr_cfg;

    //module objects
    zerr::FeatureBank *bank;
    zerr::TrajectoryGenerator *gen;
    zerr::Mapper *mapper;
    zerr::AudioRouter *router;
};

// }  //namespace zerr

/**
 * @memberof zerr
 * @brief create a new zerr instance
 * 
 * @return zerr* the new zerr instance
 */
// zerr_pd *zerr_new(sys_config *config);

/**
 * @memberof zerr
 * @brief free a zerr instance
 * 
 * @param z the zerr instance to free
 */
// void zerr_free(zerr_pd *z);

/**
 * @memberof zerr
 * @brief process a block of samples
 * 
 * @param z the zerr instance
 * @param in the input samples
 * @param out the output samples
 * @param n the number of samples
 */
// void zerr_perform(zerr_pd *z, float *in, float *out, int n);

// #ifdef __cplusplus
// }
// #endif






