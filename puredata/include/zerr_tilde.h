#pragma once

#include "m_pd.h"
#include "zerr.h"

static t_class *zerr_tilde_class;

/**
 * @struct zerr_tilde
 * @brief main pure data external
 * 
 * This is the main pure data interface
 */
typedef struct {
    t_object x_obj; /**< parent Pure Data object */
    t_float *f; /**< fallback field for the main signal inlet */

    t_outlet *sigout; /**< main signal outlet */
    t_outlet *dataout; /**< main data outlet */

    zerr *g; /**< pointer to the zerr object */

} zerr_tilde;


/**
 * @memberof zerr
 * @brief creates a new zerr_tilde object
 * 
 * @return void* a pointer to the new object or `NULL` if the creation failed
 */
void *zerr_tilde_new(void);

/**
 * @memberof zerr_tilde
 * @brief frees an existing zerr_tilde object and all of its subclasses
 * 
 * @param x the zerr object to be freed. Must not be `NULL`
 */
void zerr_tilde_free(zerr_tilde *x);

/**
 * @memberof zerr_tilde
 * @brief get a single or all parameter values from the zerr object
 * 
 * @param x the zerr object
 * @param paramname the name of the parameter to get. If `NULL` or empty, all parameters are returned
 */
void zerr_tilde_param_get(zerr_tilde *x, t_symbol *paramname);

/**
 * @memberof zerr_tilde
 * @brief updates a parameter's value
 * 
 * @param x the zerr object
 * @param paramname the name of the parameter to be updated
 * @param value the offset value to be updated
 */
void zerr_tilde_param_set(zerr_tilde *x, t_symbol *paramname, t_float value);

/**
 * @memberof zerr_tilde
 * @brief updates a parameter slot's amount of influence
 * 
 * @param x the zerr object
 * @param paramname the name of the parameter to be updated
 * @param fslot the slot to be updated
 * @param value the amount to be updated
 */
void zerr_tilde_param_amount(zerr_tilde *x, t_symbol *paramname, t_float fslot, t_float value);

/**
 * @memberof zerr_tilde
 * @brief connects a modulator to a parameter.
 * Any other modulator on that slot will be disconnected.
 * Due to a bug in the windows version of Pure Data, this method had to be implemented using
 * A_GIMME instead of the parameter list A_SYMBOL, A_FLOAT, A_SYMBOL.
 * 
 * @param x the zerr object
 * @param s unused symbol representation of the following arguments
 * @param argc the number of arguments
 * @param argv the arguments in the order: param name, slot, modulator name
 */
void zerr_tilde_param_attach(zerr_tilde *x, t_symbol *s, int argc, t_atom *argv);

/**
 * @memberof zerr_tilde
 * @brief disconnects a modulator from a parameter
 * 
 * @param x the zerr object
 * @param paramname the name of the parameter to be updated
 * @param fslot the slot to be updated
 */
void zerr_tilde_param_detach(zerr_tilde *x, t_symbol *paramname, t_float fslot);

/**
 * @memberof zerr_tilde
 * @brief posts all parameters to the debug console
 * 
 * @param x the zerr object
 */
void zerr_tilde_param_post(zerr_tilde *x);

/**
 * @memberof zerr_tilde
 * @brief resets all the parameters to default and detaches modulators
 * 
 * @param x the zerr object
 */
void zerr_tilde_param_reset(zerr_tilde *x);

/**
 * @memberof zerr_tilde
 * @brief sets up the dsp tree for a given zerr_tilde object
 * 
 * @param x the zerr object
 * @param sp the signal pointer provided by pure data
 */
void zerr_tilde_dsp(zerr_tilde *x, t_signal **sp);

/**
 * @related zerr_tilde
 * @brief sets up the zerr~ external
 */
void zerr_tilde_setup(void);
