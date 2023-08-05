#include "zerr_envelope_generator_tilde.h"

#ifdef __cplusplus
extern "C" {
#endif

static t_class *zerr_envelope_generator_tilde_class;

void *zerr_envelope_generator_tilde_new(t_symbol *s, int argc, t_atom *argv) {
    zerr_envelope_generator_tilde *x = (zerr_envelope_generator_tilde *) pd_new(zerr_envelope_generator_tilde_class);
    if (!x) return NULL;

    zerr::t_systemConfigs systemCfgs;
    systemCfgs.sample_rate = (size_t) sys_getsr();
    systemCfgs.block_size  = (size_t) sys_getblksize();

    if (argc < 2) return NULL; // no enough args to initialize object

    // only the first two args are analyzed
    if (argv[0].a_type != A_SYMBOL) return NULL; 
    char* selectionMode = strdup(atom_getsymbol(argv)->s_name);
    if (argv[1].a_type != A_SYMBOL) return NULL; 
    char* spkrCfgName = strdup(atom_getsymbol(argv+1)->s_name);

    // find the absolute path of config file
    t_canvas *canvas = canvas_getcurrent();
    char dirResult[MAXPDSTRING], *nameResult;
    if ((canvas_open(canvas, spkrCfgName, "", dirResult, &nameResult, MAXPDSTRING, 0)) < 0){
        pd_error(0, "%s: can't open", spkrCfgName);
        return NULL;
    }

    // #ifdef TESTMODE
    // t_symbol *canvas_dir = canvas_getdir(canvas);
    // char* canvas_dir_path = strdup(canvas_dir->s_name);  //TODO: add this to system config
    // post("current working directory:");
    // post(canvas_dir_path);
    // #endif //TESTMODE 
    char spkrCfgFile[MAXPDSTRING];
    strcat(spkrCfgFile, dirResult);
    #ifdef WINDOWS
    strcat(spkrCfgFile, "\\"); 
    #else
    strcat(spkrCfgFile, "/"); 
    #endif //WINDOWS
    strcat(spkrCfgFile, nameResult);

    x->z = new ZerrEnvelopeGenerator(systemCfgs, selectionMode, spkrCfgFile); 
    if (!x->z) return NULL;

    if (!x->z->initialize()) return NULL; // initialize zerr_envelope_generator and all sub-modules

    // create inlets
    x->spread_inlet = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    x->volume_inlet = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);

    // create outlets
    x->n_outlet = x->z->numOutlet; // get the number of outlets
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
        A_GIMME,0);

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



