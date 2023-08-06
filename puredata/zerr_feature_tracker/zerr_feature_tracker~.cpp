#include "zerr_feature_tracker_tilde.h"

#ifdef __cplusplus
extern "C" {
#endif

static t_class *zerr_feature_tracker_tilde_class;

void *zerr_feature_tracker_tilde_new(t_symbol *s, int argc, t_atom *argv) {
    zerr_feature_tracker_tilde *x = (zerr_feature_tracker_tilde *) pd_new(zerr_feature_tracker_tilde_class);
    if (!x) return NULL;

    if (argc < 1) return NULL;

    zerr::t_featureNames ft_names;
    ft_names.names = (char **)malloc(argc * sizeof(char *));;
    ft_names.num   = argc;

    // Process the arguments: features
    for (int i = 0; i < argc; i++) {
        if (argv[i].a_type == A_SYMBOL){
            ft_names.names[i] = strdup(atom_getsymbol(argv+i)->s_name);
        }else{
            return NULL;
        }
        post(ft_names.names[i]);
    }

    // system config to initialize zerr
    zerr::t_systemConfigs sys_cnfg;
    sys_cnfg.sample_rate = (size_t) sys_getsr();
    sys_cnfg.block_size  = (size_t) sys_getblksize();

    x->z = new ZerrFeatureTracker(sys_cnfg, ft_names);
    if (!x->z) return NULL;

    int success = x->z->initialize(); // initialize zerr and all sub-modules
    if (!success) return NULL;

    x->n_outlet = argc; // get the number of outlets: 2 * feature numbers
    x->x_vec = (t_zerrout *)getbytes(x->n_outlet * sizeof(*x->x_vec));

    t_zerrout *u;
    int i;

    for (i = 0, u = x->x_vec; i < x->n_outlet; u++, i++){
        u->u_outlet = outlet_new(&x->x_obj, &s_signal);
    }

    return (void *) x;
}

void zerr_feature_tracker_tilde_free(zerr_feature_tracker_tilde *x) {
    freebytes(x->x_vec, x->n_outlet * sizeof(*x->x_vec));
    delete x->z;
}

static t_int *zerr_feature_tracker_tilde_perform(t_int *w) {
    zerr_feature_tracker_tilde *x = (zerr_feature_tracker_tilde *) w[1];
    int n_vec     = (int) w[2];
    int n_args    = (int) w[3];

    t_sample **ports = (t_sample **) &w[4];

    // invoke the main algorithm
    x->z->perform(ports, n_vec);

    return &w[n_args+1];
}

void zerr_feature_tracker_tilde_dsp(zerr_feature_tracker_tilde *x, t_signal **sp) {
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

    dsp_addv(zerr_feature_tracker_tilde_perform, n_args, vec);
}

void zerr_feature_tracker_tilde_setup(void) {
    zerr_feature_tracker_tilde_class = class_new(gensym("zerr_feature_tracker~"),
        (t_newmethod) zerr_feature_tracker_tilde_new,
        (t_method) zerr_feature_tracker_tilde_free,
        (size_t) sizeof(zerr_feature_tracker_tilde),
        CLASS_DEFAULT,
        A_GIMME,0);

    class_addmethod(zerr_feature_tracker_tilde_class,
        (t_method) zerr_feature_tracker_tilde_dsp,
        gensym("dsp"),
        A_CANT,
        A_NULL);

    // class_sethelpsymbol(zerr_tilde_class, gensym("zerr~"));
    CLASS_MAINSIGNALIN(zerr_feature_tracker_tilde_class, zerr_feature_tracker_tilde, f);
}

#ifdef __cplusplus
}
#endif



