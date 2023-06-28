/**
 * @file zerr_speaker_mapper_tilde.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief zerr_speaker_mapper~ Pure Data External
 * @version 0.3
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 */
#pragma once

#include "m_pd.h" //PureData APIs

#include "zerr_speaker_mapper.h" 

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct zerrout
 * @brief zerr_speaker_mapper outlet structure
 * 
 * This is structure of zerr_speaker_mapper outlets for dynamic outlet creating
 */
typedef struct zerrout
{
    // t_atomtype u_type;
    t_outlet *u_outlet;
} t_zerrout;

/**
 * @struct zerr_tilde
 * @brief the structure of pure data external. This is the main pure data interface
 */
typedef struct {
    t_object  x_obj;    /**< parent Pure Data object */
    t_float   *f;       /**< fallback field for the main signal inlet */

    t_inlet *in2;
    t_inlet *in3;

    t_int     n_outlet; /**< number of zerr_speaker_mapper outlets */
    t_zerrout *x_vec;   /**< pointer to zerr_speaker_mapper outlets structure */

    ZerrSpeakerMapper      *z;       /**< pointer to the zerr_speaker_mapper object */
} zerr_speaker_mapper_tilde;

/**
 * @memberof zerr_tilde
 * @brief creates a new zerr_tilde object
 * 
 * @return void* a pointer to the new object or `NULL` if the creation failed
 */
void *zerr_speaker_mapper_tilde_new(t_symbol *s, int argc, t_atom *argv);

/**
 * @memberof zerr_speaker_mapper_tilde
 * @brief frees an existing zerr_speaker_mapper_tilde object and all of its subclasses
 * 
 * @param x the zerr_speaker_mapper object to be freed. Must not be `NULL`
 */
void zerr_speaker_mapper_tilde_free(zerr_speaker_mapper_tilde *x);

/**
 * @memberof zerr_speaker_mapper_tilde
 * @brief sets up the dsp tree for a given zerr_speaker_mapper_tilde object
 * 
 * @param x the zerr_speaker_mapper object
 * @param sp the signal pointer provided by pure data
 */
void zerr_speaker_mapper_tilde_dsp(zerr_speaker_mapper_tilde *x, t_signal **sp);

/**
 * @related zerr_speaker_mapper_tilde
 * @brief sets up the zerr_speaker_mapper~ external
 */
void zerr_speaker_mapper_tilde_setup(void);

/**
 * @memberof zerr_speaker_mapper_tilde
 * @brief get a single or all parameter values from the zerr_speaker_mapper object
 * 
 * @param x the zerr_speaker_mapper object
 * @param paramname the name of the parameter to get. If `NULL` or empty, all parameters are returned
 */
// void zerr_speaker_mapper_tilde_param_get(zerr_speaker_mapper_tilde *x, t_symbol *paramname);

/**
 * @memberof zerr_speaker_mapper_tilde
 * @brief updates a parameter's value
 * 
 * @param x the zerr_speaker_mapper object
 * @param paramname the name of the parameter to be updated
 * @param value the offset value to be updated
 */
// void zerr_speaker_mapper_tilde_param_set(zerr_speaker_mapper_tilde *x, t_symbol *paramname, t_float value);

/**
 * @memberof zerr_speaker_mapper_tilde
 * @brief updates a parameter slot's amount of influence
 * 
 * @param x the zerr_speaker_mapper object
 * @param paramname the name of the parameter to be updated
 * @param fslot the slot to be updated
 * @param value the amount to be updated
 */
// void zerr_speaker_mapper_tilde_param_amount(zerr_speaker_mapper_tilde *x, t_symbol *paramname, t_float fslot, t_float value);

/**
 * @memberof zerr_speaker_mapper_tilde
 * @brief connects a modulator to a parameter.
 * Any other modulator on that slot will be disconnected.
 * Due to a bug in the windows version of Pure Data, this method had to be implemented using
 * A_GIMME instead of the parameter list A_SYMBOL, A_FLOAT, A_SYMBOL.
 * 
 * @param x the zerr_speaker_mapper object
 * @param s unused symbol representation of the following arguments
 * @param argc the number of arguments
 * @param argv the arguments in the order: param name, slot, modulator name
 */
// void zerr_speaker_mapper_tilde_param_attach(zerr_speaker_mapper_tilde *x, t_symbol *s, int argc, t_atom *argv);

/**
 * @memberof zerr_speaker_mapper_tilde
 * @brief disconnects a modulator from a parameter
 * 
 * @param x the zerr_speaker_mapper object
 * @param paramname the name of the parameter to be updated
 * @param fslot the slot to be updated
 */
// void zerr_speaker_mapper_tilde_param_detach(zerr_speaker_mapper_tilde *x, t_symbol *paramname, t_float fslot);

/**
 * @memberof zerr_speaker_mapper_tilde
 * @brief posts all parameters to the debug console
 * 
 * @param x the zerr_speaker_mapper object
 */
// void zerr_speaker_mapper_tilde_param_post(zerr_speaker_mapper_tilde *x);

/**
 * @memberof zerr_speaker_mapper_tilde
 * @brief resets all the parameters to default and detaches modulators
 * 
 * @param x the zerr_speaker_mapper object
 */
// void zerr_speaker_mapper_tilde_param_reset(zerr_speaker_mapper_tilde *x);

#ifdef __cplusplus
}
#endif
