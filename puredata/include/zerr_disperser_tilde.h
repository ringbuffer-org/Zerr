/**
 * @file zerr_disperser_tilde.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief zerr_disperser~ Pure Data External
 * @date 2024-01-29
 *
 * @copyright Copyright (c) 2023-2024
 */
#pragma once

#include "./m_pd.h"  // PureData APIs
#include "./zerr_disperser.h"
#include "configs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct t_zerrout
 * @brief zerr_disperser~ outlet structure
 *
 * This is structure of zerr_disperser~ outlets for dynamic outlet creating
 */
typedef struct zerrout {
    t_outlet *u_outlet;
} t_zerrout;

/**
 * @struct t_zerrin
 * @brief zerr_disperser~ inlet structure
 *
 * This is structure of zerr_disperser~ inlets for dynamic inlet creating
 */
typedef struct zerrin {
    t_inlet *u_inlet;
} t_zerrin;

/**
 * @struct zerr_disperser_tilde
 * @brief the structure of pure data external. This is the main pure data
 * interface
 */
typedef struct {
    t_object x_obj; /**< parent Pure Data object */
    t_float *f;     /**< fallback field for the main signal inlet */

    t_int n_inlet;      /**< number of the object inlets */
    t_zerrin *x_in_vec; /**< pointer to the inlets structure */

    t_int n_outlet;       /**< number of the object outlets */
    t_zerrout *x_out_vec; /**< pointer to the outlets structure */

    ZerrDisperser *z; /**< zerr_disperser object */
} zerr_disperser_tilde;

/**
 * @memberof zerr_disperser_tilde
 * @brief creates a new zerr_disperser_tilde object
 *
 * @return void* a pointer to the new object or `NULL` if the creation failed
 */
void *zerr_disperser_tilde_new(t_symbol *s, int argc, t_atom *argv);

/**
 * @memberof zerr_disperser_tilde
 * @brief frees an existing zerr_disperser_tilde object and all of its
 * subclasses
 *
 * @param x the zerr_disperser object to be freed. Must not be `NULL`
 */
void zerr_disperser_tilde_free(zerr_disperser_tilde *x);

/**
 * @memberof zerr_disperser_tilde
 * @brief sets up the dsp tree for a given zerr_disperser_tilde object
 *
 * @param x the zerr_disperser object
 * @param sp the signal pointer provided by pure data
 */
void zerr_disperser_tilde_dsp(zerr_disperser_tilde *x, t_signal **sp);

/**
 * @related zerr_disperser_tilde
 * @brief sets up the zerr_disperser~ external
 */
void zerr_disperser_tilde_setup(void);

#ifdef __cplusplus
}
#endif
