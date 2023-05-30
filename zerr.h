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

#include "zerr_config.h"
#include "m_pd.h" // for testing


/**
 * @brief Main GOAT class
 */
typedef struct zerr {
    sys_config cfg; /**< Puredata config */
    // modulator_bank *modbank; /**< the modulator bank */
    // vocaldetector *vd; /**< the vocal detector */
    // granular *gran;     /**< the granular instance */
    // scheduler *schdur;  /**< the scheduler instance */
    // analyzer *anlyz;    /**< the analyzer instance */
    // transformer *trans; /**< the transformer instance */
} zerr;


/**
 * @memberof zerr
 * @brief create a new zerr instance
 * 
 * @return zerr* the new zerr instance
 */
zerr *zerr_new(sys_config *config);

/**
 * @memberof zerr
 * @brief free a zerr instance
 * 
 * @param z the zerr instance to free
 */
void zerr_free(zerr *z);

/**
 * @memberof zerr
 * @brief process a block of samples
 * 
 * @param z the zerr instance
 * @param in the input samples
 * @param out the output samples
 * @param n the number of samples
 */
void zerr_perform(zerr *z, float *in, float *out, int n);
