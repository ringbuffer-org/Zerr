/**
 * @file zerr_envelope_generator_tilde.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief zerr_envelope_generator~ Pure Data External
 * @version 0.5
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2023
 */
#pragma once

#include "m_pd.h" //PureData APIs

#include "zerr_envelope_generator.h" 
#include "configs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct zerrout
 * @brief zerr_envelope_generator outlet structure
 * 
 * This is structure of zerr_envelope_generator outlets for dynamic outlet creating
 */
typedef struct zerrout {
    t_outlet *u_outlet;
} t_zerrout;

/**
 * @struct zerr_envelope_generator_tilde
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

    ZerrEnvelopeGenerator *z;       /**< zerr_envelope_generator object */
} zerr_envelope_generator_tilde;

/**
 * @memberof zerr_envelope_generator_tilde
 * @brief creates a new zerr_envelope_generator_tilde object
 * 
 * @return void* a pointer to the new object or `NULL` if the creation failed
 */
void *zerr_envelope_generator_tilde_new(t_symbol *s, int argc, t_atom *argv);

/**
 * @memberof zerr_envelope_generator_tilde
 * @brief frees an existing zerr_envelope_generator_tilde object and all of its subclasses
 * 
 * @param x the zerr_envelope_generator object to be freed. Must not be `NULL`
 */
void zerr_envelope_generator_tilde_free(zerr_envelope_generator_tilde *x);

/**
 * @memberof zerr_envelope_generator_tilde
 * @brief sets up the dsp tree for a given zerr_envelope_generator_tilde object
 * 
 * @param x the zerr_envelope_generator object
 * @param sp the signal pointer provided by pure data
 */
void zerr_envelope_generator_tilde_dsp(zerr_envelope_generator_tilde *x, t_signal **sp);

/**
 * @related zerr_envelope_generator_tilde
 * @brief sets up the zerr_envelope_generator~ external
 */
void zerr_envelope_generator_tilde_setup(void);


#ifdef __cplusplus
}
#endif

