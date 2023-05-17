/**
 * @file zerr.h
 * @author Amon Benson (amonkbenson@gmail.com)
 * @brief Main zerr class
 * @version 0.1
 * @date 2021-07-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "zerr_config.h"
#include "granular/granular.h"
#include "scheduler/scheduler.h"
#include "modulators/modulator_bank.h"
#include "control/manager.h"
#include "pitch/vocaldetector.h"

#include "params.h"

/**
 * @brief Main zerr class
 */
typedef struct zerr {
    zerr_config cfg; /**< configuration parameters */
    modulator_bank *modbank; /**< the modulator bank */
    vocaldetector *vd; /**< the vocal detector */
    granular *gran;     /**< the granular instance */
    scheduler *schdur;  /**< the scheduler instance */
    // analyzer *anlyz;    /**< the analyzer instance */
    // transformer *trans; /**< the transformer instance */
} zerr;


/**
 * @memberof zerr
 * @brief create a new zerr instance
 * 
 * @return zerr* the new zerr instance
 */
zerr *zerr_new(zerr_config *config);

/**
 * @memberof zerr
 * @brief free a zerr instance
 * 
 * @param g the zerr instance to free
 */
void zerr_free(zerr *g);

/**
 * @memberof zerr
 * @brief process a block of samples
 * 
 * @param g the zerr instance
 * @param in the input samples
 * @param out the output samples
 * @param n the number of samples
 */
void zerr_perform(zerr *g, float *in, float *out, int n);
