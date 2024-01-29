#include "zerr_combinator_tilde.h"

#ifdef __cplusplus
extern "C" {
#endif

static t_class *zerr_combinator_tilde_class;

void *zerr_combinator_tilde_new(t_symbol *s, int argc, t_atom *argv) {
    zerr_combinator_tilde *x = (zerr_combinator_tilde *) pd_new(zerr_combinator_tilde_class);
    if (!x) return NULL;

    zerr::t_systemConfigs systemCfgs;
    systemCfgs.sample_rate = (size_t) sys_getsr();
    systemCfgs.block_size  = (size_t) sys_getblksize();

    if (argc < 3) return NULL; // no enough args to initialize object

    // only the first two args are analyzed
    if (argv[0].a_type != A_FLOAT) return NULL; 
    int numSource  = (int)argv[0].a_w.w_float;
    if (argv[1].a_type != A_FLOAT) return NULL; 
    int numChannel = (int)argv[1].a_w.w_float;
    if (argv[2].a_type != A_SYMBOL) return NULL; 
    char* combinationMode = strdup(atom_getsymbol(argv+2)->s_name);

    x->z = new ZerrEnvelopeCombinator(numSource, numChannel, systemCfgs, combinationMode); // CfgFile
    if (!x->z) return NULL;

    if (!x->z->initialize()) return NULL;

    // create inlets
    x->n_inlet = numSource * numChannel;    // exclude the main inlet
    x->x_in_vec = (t_zerrin *)getbytes((x->n_inlet-1) * sizeof(*x->x_in_vec));
    t_zerrin *u_in;
    int cntIn;
    for (cntIn = 1, u_in = x->x_in_vec; cntIn < x->n_inlet; u_in++, cntIn++){
        u_in->u_inlet = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    }

    // create outlets
    x->n_outlet = numChannel; // get the number of outlets
    x->x_out_vec = (t_zerrout *)getbytes(x->n_outlet * sizeof(*x->x_out_vec));
    t_zerrout *u_out;
    int cntOut;
    for (cntOut = 0, u_out = x->x_out_vec; cntOut < x->n_outlet; u_out++, cntOut++){
        u_out->u_outlet = outlet_new(&x->x_obj, &s_signal);
    }

    return (void *) x;
}


void zerr_combinator_tilde_free(zerr_combinator_tilde *x) {
    freebytes(x->x_in_vec, x->n_inlet * sizeof(*x->x_in_vec));
    freebytes(x->x_out_vec, x->n_outlet * sizeof(*x->x_out_vec));
    delete x->z;
}


static t_int *zerr_combinator_tilde_perform(t_int *w) {
    zerr_combinator_tilde *x = (zerr_combinator_tilde *) w[1];
    int n_vec     = (int) w[2];
    int n_args    = (int) w[3];

    t_sample **ports = (t_sample **) &w[4];

    // invoke the main algorithm
    x->z->perform(ports, n_vec);

    return &w[n_args+1];
}


void zerr_combinator_tilde_dsp(zerr_combinator_tilde *x, t_signal **sp) {
    int n_rest = 3; // size of [x, n_vec, n_args]

    int n_vec = sp[0]->s_n;
    int n_port = x->n_outlet + x->n_inlet;
    int n_args = n_port + n_rest;

    t_int *vec = (t_int *) getbytes(n_args * sizeof(t_int *));

    vec[0] = (t_int) x;
    vec[1] = (t_int) n_vec;
    vec[2] = (t_int) n_args;
    for (int i = 0; i < n_port; ++i){
        vec[i+n_rest] = (t_int) sp[i]->s_vec;
    }

    dsp_addv(zerr_combinator_tilde_perform, n_args, vec);
}


void zerr_combinator_tilde_setup(void) {
    zerr_combinator_tilde_class = class_new(gensym("zerr_combinator~"),
        (t_newmethod) zerr_combinator_tilde_new,
        (t_method) zerr_combinator_tilde_free,
        (size_t) sizeof(zerr_combinator_tilde),
        CLASS_DEFAULT,
        A_GIMME,0);

    class_addmethod(zerr_combinator_tilde_class,
        (t_method) zerr_combinator_tilde_dsp,
        gensym("dsp"),
        A_CANT,
        A_NULL);

    // class_sethelpsymbol(zerr_combinator_tilde_class, gensym("zerr_combinator~"));
    CLASS_MAINSIGNALIN(zerr_combinator_tilde_class, zerr_combinator_tilde, f);
}


#ifdef __cplusplus
}
#endif



