/**
 * @file zerr_envelopes_tilde.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief zerr_envelopes~ Pure Data External
 * @version 0.5
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2023
 */
#pragma once

#include "m_pd.h" //PureData APIs

#include "zerr_envelopes.h" 
#include "configs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct zerrout
 * @brief zerr_envelopes outlet structure
 * 
 * This is structure of zerr_envelopes outlets for dynamic outlet creating
 */
typedef struct zerrout {
    t_outlet *u_outlet;
} t_zerrout;

/**
 * @struct zerr_envelopes_tilde
 * @brief the structure of pure data external. This is the main pure data interface
 */
typedef struct {
    t_object  x_obj; /**< parent Pure Data object */
    t_float   *f;    /**< fallback field for the main signal inlet */

    //t_inlet *main_inlet; /The main inlet is created by PD automatically
    t_inlet *spread_inlet; /**< inlet to receive spread control signal */
    t_inlet *volume_inlet; /**< inlet to receive volume control signal */

    t_int      n_outlet; /**< number of the object outlets */
    t_zerrout  *x_vec;   /**< pointer to the outlets structure */

    ZerrEnvelopeGenerator *z;       /**< zerr_envelopes object */
} zerr_envelopes_tilde;

/**
 * @memberof zerr_envelopes_tilde
 * @brief creates a new zerr_envelopes_tilde object
 * 
 * @return void* a pointer to the new object or `NULL` if the creation failed
 */
void *zerr_envelopes_tilde_new(t_symbol *s, int argc, t_atom *argv);

/**
 * @memberof zerr_envelopes_tilde
 * @brief frees an existing zerr_envelopes_tilde object and all of its subclasses
 * 
 * @param x the zerr_envelopes object to be freed. Must not be `NULL`
 */
void zerr_envelopes_tilde_free(zerr_envelopes_tilde *x);

/**
 * @memberof zerr_envelopes_tilde
 * @brief sets up the dsp tree for a given zerr_envelopes_tilde object
 * 
 * @param x the zerr_envelopes object
 * @param sp the signal pointer provided by pure data
 */
void zerr_envelopes_tilde_dsp(zerr_envelopes_tilde *x, t_signal **sp);

/**
 * @related zerr_envelopes_tilde
 * @brief sets up the zerr_envelopes~ external
 */
void zerr_envelopes_tilde_setup(void);

/**
 * @memberof zerr_envelopes_tilde
 * @brief updates a parameter's value
 * 
 * @param x the zerr_envelopes object
 * @param paramname the name of the parameter to be updated
 * @param value the offset value to be updated
 */
void zerr_envelopes_tilde_active_speakers(zerr_envelopes_tilde *x, t_symbol *s, int argc, t_atom *argv);

/**
 * @memberof zerr_envelopes_tilde
 * @brief updates a parameter's value
 * 
 * @param x the zerr_envelopes object
 * @param paramname the name of the parameter to be updated
 * @param value the offset value to be updated
 */
void zerr_envelopes_tilde_trajectory(zerr_envelopes_tilde *x, t_symbol *s, int argc, t_atom *argv);

/**
 * @memberof zerr_envelopes_tilde
 * @brief posts all parameters to the debug console
 * 
 * @param x the zerr_envelopes object
 */
void zerr_envelopes_tilde_print(zerr_envelopes_tilde *x, t_symbol *paramname);

/**
 * @memberof zerr_envelopes_tilde
 * @brief resets all the parameters to default
 * 
 * @param x the zerr_envelopes object
 */
void zerr_envelopes_tilde_param_reset(zerr_envelopes_tilde *x);

#ifdef __cplusplus
}
#endif

