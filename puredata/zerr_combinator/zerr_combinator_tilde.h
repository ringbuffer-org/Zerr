/**
 * @file zerr_combinator_tilde.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief zerr_combinator~ Pure Data External
 * @date 2024-01-29
 * 
 * @copyright Copyright (c) 2023-2024
 */
#pragma once

#include "m_pd.h"

#include "zerr_combinator.h"
#include "configs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct zerrout
 * @brief zerr_combinator~ outlet structure
 * 
 * This is structure of zerr_combinator~ outlets for dynamic outlet creating
 */
typedef struct zerrout {
    t_outlet *u_outlet;
} t_zerrout;

/**
 * @struct zerrin
 * @brief zerr_combinator~ inlet structure
 * 
 * This is structure of zerr_combinator~ inlets for dynamic inlet creating
 */
typedef struct zerrin {
    t_inlet *u_inlet;
} t_zerrin;

/**
 * @struct zerr_combinator_tilde
 * @brief the structure of pure data external. This is the main pure data interface
 */
typedef struct {
    t_object  x_obj; /**< parent Pure Data object */
    t_float   *f;    /**< fallback field for the main signal inlet */

    t_int      n_inlet;     /**< number of the object inlets */
    t_zerrin   *x_in_vec;   /**< pointer to the inlets structure */

    t_int      n_outlet;    /**< number of the object outlets */
    t_zerrout  *x_out_vec;   /**< pointer to the outlets structure */

    ZerrCombinator *z;       /**< zerr_combinator core component */
} zerr_combinator_tilde;

/**
 * @memberof zerr_combinator_tilde
 * @brief creates a new zerr_combinator_tilde object
 * 
 * @return void* a pointer to the new object or `NULL` if the creation failed
 */
void *zerr_combinator_tilde_new(t_symbol *s, int argc, t_atom *argv);

/**
 * @memberof zerr_combinator_tilde
 * @brief frees an existing zerr_combinator_tilde object and all of its subclasses
 * 
 * @param x the zerr_combinator~ object to be freed. Must not be `NULL`
 */
void zerr_combinator_tilde_free(zerr_combinator_tilde *x);

/**
 * @memberof zerr_combinator_tilde
 * @brief sets up the dsp tree for a given zerr_combinator_tilde object
 * 
 * @param x the zerr_combinator~ object
 * @param sp the signal pointer provided by pure data
 */
void zerr_combinator_tilde_dsp(zerr_combinator_tilde *x, t_signal **sp);

/**
 * @related zerr_combinator_tilde
 * @brief sets up the zerr_combinator~ external
 */
void zerr_combinator_tilde_setup(void);


#ifdef __cplusplus
}
#endif



