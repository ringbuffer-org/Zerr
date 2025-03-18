/**
 * @file zerr_combinator_tilde.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief zerr_combinator~ Pure Data External - Header file containing the main interface definitions
 * @date 2024-01-29
 *
 * @copyright Copyright (c) 2023-2024
 */
#pragma once

#include "configs.h"
#include "m_pd.h"
#include "zerr_combinator.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct zerrout
 * @brief zerr_combinator~ outlet structure
 *
 * This structure defines the outlets for zerr_combinator~ external. It allows for dynamic creation
 * of outlets that can output signal or control data from the zerr_combinator processing chain.
 */
typedef struct zerrout {
    t_outlet *u_outlet;  /**< Pure Data outlet pointer */
} t_zerrout;

/**
 * @struct zerrin
 * @brief zerr_combinator~ inlet structure
 *
 * This structure defines the inlets for zerr_combinator~ external. It allows for dynamic creation
 * of inlets that can receive signal or control data for the zerr_combinator processing chain.
 */
typedef struct zerrin {
    t_inlet *u_inlet;  /**< Pure Data inlet pointer */
} t_zerrin;

/**
 * @struct zerr_combinator_tilde
 * @brief The main Pure Data external interface structure
 * 
 * This structure represents the complete state of a zerr_combinator~ object instance.
 * It contains all necessary components for audio processing, data routing, and 
 * interaction with Pure Data's signal processing system.
 */
typedef struct {
    t_object x_obj;     /**< Parent Pure Data object - must be the first member */
    t_float *f;         /**< Fallback field for the main signal inlet when no signal is connected */

    t_int n_inlet;      /**< Number of inlets created for this instance */
    t_zerrin *x_in_vec; /**< Dynamic array of inlet structures */

    t_int n_outlet;     /**< Number of outlets created for this instance */
    t_zerrout *x_out_vec; /**< Dynamic array of outlet structures */

    ZerrCombinator *z;  /**< Pointer to the core zerr_combinator processing component */
} zerr_combinator_tilde;

/**
 * @memberof zerr_combinator_tilde
 * @brief Creates a new zerr_combinator_tilde object instance
 * 
 * This function handles the instantiation of a new zerr_combinator~ object,
 * including memory allocation, initialization of inlets/outlets, and setup of
 * the core processing component.
 *
 * @param s Symbol containing the object name (unused)
 * @param argc Number of creation arguments
 * @param argv Array of creation arguments
 * @return void* Pointer to the new object or NULL if creation failed
 */
void *zerr_combinator_tilde_new(t_symbol *s, int argc, t_atom *argv);

/**
 * @memberof zerr_combinator_tilde
 * @brief Frees all resources associated with a zerr_combinator_tilde object
 *
 * This function handles proper cleanup of the object, including freeing memory
 * for inlets, outlets, and the core processing component.
 *
 * @param x Pointer to the zerr_combinator~ object to be freed (must not be NULL)
 */
void zerr_combinator_tilde_free(zerr_combinator_tilde *x);

/**
 * @memberof zerr_combinator_tilde
 * @brief Sets up the DSP processing chain for the object
 *
 * This function is called by Pure Data when the DSP chain is being configured.
 * It establishes the necessary signal processing connections and prepares the
 * object for audio processing.
 *
 * @param x Pointer to the zerr_combinator~ object
 * @param sp Array of signal pointers provided by Pure Data
 */
void zerr_combinator_tilde_dsp(zerr_combinator_tilde *x, t_signal **sp);

/**
 * @related zerr_combinator_tilde
 * @brief Initializes the zerr_combinator~ external in Pure Data
 * 
 * This setup function is called when the external is loaded into Pure Data.
 * It registers the class, methods, and properties of the zerr_combinator~ object
 * with Pure Data's runtime system.
 */
void zerr_combinator_tilde_setup(void);

#ifdef __cplusplus
}
#endif
