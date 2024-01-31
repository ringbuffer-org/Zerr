/**
 * @file zerr_features_tilde.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief zerr_features~ Pure Data External
 * @date 2024-01-30
 * 
 * @copyright Copyright (c) 2023-2024
 */
#pragma once

#include "m_pd.h"

#include "./zerr_features.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct zerrout
 * @brief zerr_features outlet structure
 * 
 * This is structure of zerr_features outlets for dynamic outlet creating
 */
typedef struct zerrout {
    t_outlet *u_outlet;
} t_zerrout;

/**
 * @struct zerr_features_tilde
 * @brief the structure of pure data external. This is the main pure data interface
 */
typedef struct {
    t_object      x_obj;    /**< parent Pure Data object */
    t_float      *f;        /**< fallback field for the main signal inlet */
    t_int         n_outlet; /**< number of zerr outlets */
    t_zerrout    *x_vec;    /**< pointer to zerr outlets structure */

    ZerrFeatures *z;        /**< pointer to the ZerrFeatures object */
} zerr_features_tilde;

/**
 * @memberof zerr_features_tilde
 * @brief creates a new zerr_features_tilde object
 * 
 * @return void* a pointer to the new object or `NULL` if the creation failed
 */
void *zerr_features_tilde_new(t_symbol *s, int argc, t_atom *argv);

/**
 * @memberof zerr_features_tilde
 * @brief frees an existing zerr_features_tilde object and all of its subclasses
 * 
 * @param x the zerr_features object to be freed. Must not be `NULL`
 */
void zerr_features_tilde_free(zerr_features_tilde *x);

/**
 * @memberof zerr_features_tilde
 * @brief sets up the dsp tree for a given zerr_features_tilde object
 * 
 * @param x the zerr object
 * @param sp the signal pointer provided by pure data
 */
void zerr_features_tilde_dsp(zerr_features_tilde *x, t_signal **sp);

/**
 * @related zerr_features_tilde
 * @brief sets up the zerr~ external
 */
void zerr_features_tilde_setup(void);

#ifdef __cplusplus
}
#endif

