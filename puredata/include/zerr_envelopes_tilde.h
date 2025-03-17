/**
 * @file zerr_envelopes_tilde.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief zerr_envelopes~ Pure Data External - Header file containing the main interface definitions
 * @date 2024-01-30
 *
 * @copyright Copyright (c) 2023-2024
 */
#pragma once

#include "./zerr_envelopes.h"
#include "configs.h"
#include "m_pd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct zerrout
 * @brief zerr_envelopes~ outlet structure
 *
 * This structure defines the outlets for zerr_envelopes~ external. It allows for dynamic creation
 * of outlets that can output signal or control data from the zerr_envelopes processing chain.
 */
typedef struct zerrout {
    t_outlet *u_outlet;  /**< Pure Data outlet pointer */
} t_zerrout;

/**
 * @struct zerr_envelopes_tilde
 * @brief The main Pure Data external interface structure
 * 
 * This structure represents the complete state of a zerr_envelopes~ object instance.
 * It contains all necessary components for audio processing, data routing, and 
 * interaction with Pure Data's signal processing system.
 */
typedef struct {
    t_object x_obj; /**< Parent Pure Data object - must be the first member */
    t_float *f;     /**< Fallback field for the main signal inlet when no signal is connected */

    // The main inlet is created by PD automatically
    t_inlet *spread_inlet; /**< Inlet for receiving spread control signals to adjust envelope spread */
    t_inlet *volume_inlet; /**< Inlet for receiving volume control signals to adjust envelope amplitude */

    t_int n_outlet;   /**< Number of outlets created for this instance */
    t_zerrout *x_vec; /**< Dynamic array of outlet structures */

    ZerrEnvelopes *z; /**< Pointer to the core zerr_envelopes processing component */
} zerr_envelopes_tilde;

/**
 * @memberof zerr_envelopes_tilde
 * @brief Creates a new zerr_envelopes_tilde object instance
 * 
 * This function handles the instantiation of a new zerr_envelopes~ object,
 * including memory allocation, initialization of inlets/outlets, and setup of
 * the core processing component.
 *
 * @param s Symbol containing the object name (unused)
 * @param argc Number of creation arguments
 * @param argv Array of creation arguments
 * @return void* Pointer to the new object or NULL if creation failed
 */
void *zerr_envelopes_tilde_new(t_symbol *s, int argc, t_atom *argv);

/**
 * @memberof zerr_envelopes_tilde
 * @brief Frees all resources associated with a zerr_envelopes_tilde object
 *
 * This function handles proper cleanup of the object, including freeing memory
 * for inlets, outlets, and the core processing component.
 *
 * @param x Pointer to the zerr_envelopes~ object to be freed (must not be NULL)
 */
void zerr_envelopes_tilde_free(zerr_envelopes_tilde *x);

/**
 * @memberof zerr_envelopes_tilde
 * @brief Sets up the DSP processing chain for the object
 *
 * This function is called by Pure Data when the DSP chain is being configured.
 * It establishes the necessary signal processing connections and prepares the
 * object for audio processing.
 *
 * @param x Pointer to the zerr_envelopes~ object
 * @param sp Array of signal pointers provided by Pure Data
 */
void zerr_envelopes_tilde_dsp(zerr_envelopes_tilde *x, t_signal **sp);

/**
 * @related zerr_envelopes_tilde
 * @brief Initializes the zerr_envelopes~ external in Pure Data
 * 
 * This setup function is called when the external is loaded into Pure Data.
 * It registers the class, methods, and properties of the zerr_envelopes~ object
 * with Pure Data's runtime system.
 */
void zerr_envelopes_tilde_setup(void);

/**
 * @memberof zerr_envelopes_tilde
 * @brief Updates the list of active speakers in the envelope system
 * 
 * This method processes incoming messages to update which speakers are currently
 * active in the envelope generation system.
 *
 * @param x Pointer to the zerr_envelopes~ object
 * @param s Symbol containing the message selector (unused)
 * @param argc Number of arguments in the message
 * @param argv Array of arguments specifying active speakers
 */
void zerr_envelopes_tilde_active_speakers(zerr_envelopes_tilde *x, t_symbol *s,
                                          int argc, t_atom *argv);

/**
 * @memberof zerr_envelopes_tilde
 * @brief Updates the trajectory parameters for envelope movement
 * 
 * This method processes incoming messages to update the trajectory path
 * that the envelopes will follow during processing.
 *
 * @param x Pointer to the zerr_envelopes~ object
 * @param s Symbol containing the message selector (unused)
 * @param argc Number of arguments in the message
 * @param argv Array of arguments specifying trajectory parameters
 */
void zerr_envelopes_tilde_trajectory(zerr_envelopes_tilde *x, t_symbol *s,
                                     int argc, t_atom *argv);

/**
 * @memberof zerr_envelopes_tilde
 * @brief Sets the trigger interval for envelope generation
 * 
 * This method configures how frequently new envelopes are triggered
 * in the processing chain.
 *
 * @param x Pointer to the zerr_envelopes~ object
 * @param s Symbol containing the message selector (unused)
 * @param argc Number of arguments in the message
 * @param argv Array of arguments specifying interval parameters
 */
void zerr_envelopes_tilde_trigger_interval(zerr_envelopes_tilde *x, t_symbol *s,
                                           int argc, t_atom *argv);

/**
 * @memberof zerr_envelopes_tilde
 * @brief Outputs the current state information to the Pure Data console
 * 
 * This debug method prints the current configuration and state of the
 * zerr_envelopes~ object to Pure Data's console window.
 *
 * @param x Pointer to the zerr_envelopes~ object
 * @param s Symbol containing the message selector (unused)
 */
void zerr_envelopes_tilde_print(zerr_envelopes_tilde *x, t_symbol *s);

/**
 * @memberof zerr_envelopes_tilde
 * @brief Resets all parameters to their default values
 *
 * This method restores all configurable parameters of the zerr_envelopes~
 * object to their initial default states.
 *
 * @param x Pointer to the zerr_envelopes~ object
 */
void zerr_envelopes_tilde_param_reset(zerr_envelopes_tilde *x);

#ifdef __cplusplus
}
#endif
