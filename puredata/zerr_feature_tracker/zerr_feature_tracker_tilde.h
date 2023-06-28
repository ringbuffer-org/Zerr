/**
 * @file zerr_feature_tracker_tilde.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief zerr_feature_tracker~ Pure Data External
 * @version 0.4
 * @date 2023-06-27
 * 
 * @copyright Copyright (c) 2023
 */
#pragma once

#include "m_pd.h" //PureData APIs

#include "zerr_feature_tracker.h" 

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct zerrout
 * @brief zerr_feature_tracker outlet structure
 * 
 * This is structure of zerr_feature_tracker outlets for dynamic outlet creating
 */
typedef struct zerrout
{
    // t_atomtype u_type;
    t_outlet *u_outlet;
} t_zerrout;

/**
 * @struct zerr_feature_tracker_tilde
 * @brief the structure of pure data external. This is the main pure data interface
 */
typedef struct {
    t_object            x_obj;    /**< parent Pure Data object */
    t_float            *f;        /**< fallback field for the main signal inlet */
    t_int               n_outlet; /**< number of zerr outlets */
    t_zerrout          *x_vec;    /**< pointer to zerr outlets structure */

    ZerrFeatureTracker *z;        /**< pointer to the zerr object */
} zerr_feature_tracker_tilde;

/**
 * @memberof zerr_feature_tracker_tilde
 * @brief creates a new zerr_feature_tracker_tilde object
 * 
 * @return void* a pointer to the new object or `NULL` if the creation failed
 */
void *zerr_feature_tracker_tilde_new(t_symbol *s, int argc, t_atom *argv);

/**
 * @memberof zerr_feature_tracker_tilde
 * @brief frees an existing zerr_feature_tracker_tilde object and all of its subclasses
 * 
 * @param x the zerr_feature_tracker object to be freed. Must not be `NULL`
 */
void zerr_feature_tracker_tilde_free(zerr_feature_tracker_tilde *x);

/**
 * @memberof zerr_feature_tracker_tilde
 * @brief sets up the dsp tree for a given zerr_feature_tracker_tilde object
 * 
 * @param x the zerr object
 * @param sp the signal pointer provided by pure data
 */
void zerr_feature_tracker_tilde_dsp(zerr_feature_tracker_tilde *x, t_signal **sp);

/**
 * @related zerr_feature_tracker_tilde
 * @brief sets up the zerr~ external
 */
void zerr_feature_tracker_tilde_setup(void);

#ifdef __cplusplus
}
#endif

