#include "zerr_envelope_generator_tilde.h"


#ifdef __cplusplus
extern "C" {
#endif

static t_class *zerr_envelope_generator_tilde_class;


void *zerr_envelope_generator_tilde_new(t_symbol *s, int argc, t_atom *argv) {
    zerr_envelope_generator_tilde *x = (zerr_envelope_generator_tilde *) pd_new(zerr_envelope_generator_tilde_class);
    if (!x) return NULL;

    zerr::t_systemConfigs sys_cnfg;
    sys_cnfg.sample_rate = (size_t) sys_getsr();
    sys_cnfg.block_size  = (size_t) sys_getblksize();

    // remove this
    std::string spkrCfgFile = "/Users/zeyuyang42/Documents/workspace/Zerr/configs/spkr_configs/eruption_8.yaml"; 

    x->z = new ZerrEnvelopeGenerator(sys_cnfg, spkrCfgFile);
    if (!x->z) return NULL;

    x->z->initialize(); // initialize zerr_envelope_generator and all sub-modules

    // inlets
    x->in2 = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    x->in3 = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);

    // outlets
    x->n_outlet = x->z->n_outlet; // get the number of outlets
    x->x_vec = (t_zerrout *)getbytes(x->n_outlet * sizeof(*x->x_vec));

    t_zerrout *u;
    int i;

    for (i = 0, u = x->x_vec; i < x->n_outlet; u++, i++){
        u->u_outlet = outlet_new(&x->x_obj, &s_signal);
    }

    return (void *) x;
}


void zerr_envelope_generator_tilde_free(zerr_envelope_generator_tilde *x) {
    freebytes(x->x_vec, x->n_outlet * sizeof(*x->x_vec));
    delete x->z;
}


static t_int *zerr_envelope_generator_tilde_perform(t_int *w) {
    zerr_envelope_generator_tilde *x = (zerr_envelope_generator_tilde *) w[1];
    int n_vec     = (int) w[2];
    int n_args    = (int) w[3];

    t_sample **ports = (t_sample **) &w[4];

    // invoke the main algorithm
    x->z->perform(ports, n_vec);

    return &w[n_args+1];
}


void zerr_envelope_generator_tilde_dsp(zerr_envelope_generator_tilde *x, t_signal **sp) {
    int n_rest = 3; // size of [x, n_vec, n_args]

    int n_vec = sp[0]->s_n;
    int n_port = x->z->get_port_count();
    int n_args = n_port + n_rest;

    t_int *vec = (t_int *) getbytes(n_args * sizeof(t_int *));

    vec[0] = (t_int) x;
    vec[1] = (t_int) n_vec;
    vec[2] = (t_int) n_args;
    for (int i = 0; i < n_port; ++i){
        vec[i+n_rest] = (t_int) sp[i]->s_vec;
    }

    dsp_addv(zerr_envelope_generator_tilde_perform, n_args, vec);
}


void zerr_envelope_generator_tilde_setup(void) {
    zerr_envelope_generator_tilde_class = class_new(gensym("zerr_envelope_generator~"),
        (t_newmethod) zerr_envelope_generator_tilde_new,
        (t_method) zerr_envelope_generator_tilde_free,
        (size_t) sizeof(zerr_envelope_generator_tilde),
        CLASS_DEFAULT,
        A_NULL);

    class_addmethod(zerr_envelope_generator_tilde_class,
        (t_method) zerr_envelope_generator_tilde_dsp,
        gensym("dsp"),
        A_CANT,
        A_NULL);

    // class_sethelpsymbol(zerr_envelope_generator_tilde_class, gensym("zerr_envelope_generator~"));
    CLASS_MAINSIGNALIN(zerr_envelope_generator_tilde_class, zerr_envelope_generator_tilde, f);
}


#ifdef __cplusplus
}
#endif



