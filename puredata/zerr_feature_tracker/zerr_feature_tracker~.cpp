#include "zerr_feature_tracker_tilde.h"
// #include "zerr.h"

#ifdef __cplusplus
extern "C" {
#endif

static t_class *zerr_feature_tracker_tilde_class;

void *zerr_feature_tracker_tilde_new(t_symbol *s, int argc, t_atom *argv) {
    zerr_feature_tracker_tilde *x = (zerr_feature_tracker_tilde *) pd_new(zerr_feature_tracker_tilde_class);
    if (!x) return NULL;

    // system config to initialize zerr
    // SystemConfig sys_cnfg((int) sys_getsr(), (int) sys_getblksize());
    t_systemConfigs sys_cnfg;
    sys_cnfg.sample_rate = (size_t) sys_getsr();
    sys_cnfg.block_size  = (size_t) sys_getblksize();

    std::string spkrCfgFile = "/Users/yangzeyu/Downloads/Zerr/configs/spkr_configs/EN325_21.yaml"; // random_12 circulation_8

    x->z = new ZerrFeatureTracker(sys_cnfg, spkrCfgFile);
    if (!x->z) return NULL;

    x->z->initialize(); // initialize zerr and all sub-modules

    x->n_outlet = x->z->n_outlet; // get the number of outlets
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
        A_NULL);

    // class_addmethod(zerr_tilde_class,
    //     (t_method) goat_tilde_graintable_get,
    //     gensym("graintable-get"),
    //     A_NULL);

    // class_addmethod(goat_tilde_class,
    //     (t_method) goat_tilde_param_get,
    //     gensym("param-get"),
    //     A_DEFSYMBOL,
    //     A_NULL);

    // class_addmethod(goat_tilde_class,
    //     (t_method) goat_tilde_param_set,
    //     gensym("param-set"),
    //     A_SYMBOL,
    //     A_FLOAT,
    //     A_NULL);
    // class_addmethod(goat_tilde_class,
    //     (t_method) goat_tilde_param_amount,
    //     gensym("param-amount"),
    //     A_SYMBOL,
    //     A_FLOAT,
    //     A_FLOAT,
    //     A_NULL);
    // // there seems to be a bug in pure data where a float followed by a symbol argument
    // // causes Pd to crash on windows. Therefore, we use a gimme instead.
    // class_addmethod(goat_tilde_class,
    //     (t_method) goat_tilde_param_attach,
    //     gensym("param-attach"),
    //     A_GIMME,
    //     A_NULL);
    // class_addmethod(goat_tilde_class,
    //     (t_method) goat_tilde_param_detach,
    //     gensym("param-detach"),
    //     A_SYMBOL,
    //     A_FLOAT,
    //     A_NULL);
    // class_addmethod(goat_tilde_class,
    //     (t_method) goat_tilde_param_post,
    //     gensym("param-post"),
    //     A_NULL);
    // class_addmethod(goat_tilde_class,
    //     (t_method) goat_tilde_param_reset,
    //     gensym("param-reset"),
    //     A_NULL);

    class_addmethod(zerr_feature_tracker_tilde_class,
        (t_method) zerr_feature_tracker_tilde_dsp,
        gensym("dsp"),
        A_CANT,
        A_NULL);

    // class_sethelpsymbol(zerr_tilde_class, gensym("zerr~"));
    CLASS_MAINSIGNALIN(zerr_feature_tracker_tilde_class, zerr_feature_tracker_tilde, f);
}

// static control_parameter *goat_tilde_validate_parameter(goat_tilde *x, const char *name) {
//     control_parameter *param = control_manager_parameter_by_name(x->g->cfg.mgr, name);
//     if (param == NULL) error("goat~: unknown parameter %s", name);
//     return param;
// }

// static control_modulator *goat_tilde_validate_modulator(goat_tilde *x, const char *name) {
//     control_modulator *mod = control_manager_modulator_by_name(x->g->cfg.mgr, name);
//     if (mod == NULL) error("goat~: unknown modulator %s", name);
//     return mod;
// }

// static int goat_tilde_validate_slot(int slot) {
//     if (slot < 0 || slot >= CONTROL_NUM_SLOTS) {
//         error("goat~: slot %d out of range", slot);
//         return -1;
//     }
//     return slot;
// }

// void goat_tilde_graintable_get(goat_tilde *x) {
//     granular *gran = x->g->gran;
//     int buffersize = gran->buffer->size;
//     int writepos = gran->buffer->writetap.position;
//     grain *gn;
//     activategrain *agn;

//     int i;
//     int argc = 4;
//     t_atom argv[argc];

//     outlet_anything(x->dataout, gensym("graintable"), 0, NULL);

//     // inactive grains
//     for (i = 0; i < graintable_get_len(gran->grains); i++) {
//         gn = &gran->grains->data[(gran->grains->front+i) % gran->grains->size];

//         SETFLOAT(&argv[0], 0); // inactive
//         SETFLOAT(&argv[1], CIRCBUF_DIST(gn->position, writepos, buffersize)
//             / (float) buffersize); // position
//         SETFLOAT(&argv[2], gn->duration / (float) buffersize); // duration
//         SETFLOAT(&argv[3], gn->speed); // speed

//         outlet_anything(x->dataout, gensym("grain"), argc, argv);
//     }

//     // active grains
//     for (i = 0; i < gran->synth->length; i++) {
//         agn = gran->synth->data[i];
//         if (agn == NULL) continue;
//         gn = &agn->origin;

//         SETFLOAT(&argv[0], 1); // active
//         SETFLOAT(&argv[1], CIRCBUF_DIST(gn->position, writepos, buffersize)
//             / (float) buffersize); // position
//         SETFLOAT(&argv[2], gn->duration / (float) buffersize); // duration
//         SETFLOAT(&argv[3], gn->speed); // speed

//         outlet_anything(x->dataout, gensym("grain"), argc, argv);
//     }
// }

// void goat_tilde_param_get(goat_tilde *x, t_symbol *paramname) {
//     control_parameter *param;

//     // get all parameters
//     if (paramname == NULL || paramname->s_name == NULL || paramname->s_name[0] == '\0') {
//         LL_FOREACH(x->g->cfg.mgr->parameters, param) {
//             goat_tilde_param_get(x, gensym(param->name));
//         }

//         return;
//     }

//     if ((param = goat_tilde_validate_parameter(x, paramname->s_name)) == NULL) return;

//     int argc = 2;
//     t_atom argv[argc];
//     SETSYMBOL(&argv[0], paramname);
//     SETFLOAT(&argv[1], param->offset);
//     outlet_anything(x->dataout, gensym("param-get"), argc, argv);
// }

// void goat_tilde_param_set(goat_tilde *x, t_symbol *paramname, t_float value) {
//     control_parameter *param;
//     if ((param = goat_tilde_validate_parameter(x, paramname->s_name)) == NULL) return;

//     control_parameter_set(param, value);
// }

// void goat_tilde_param_amount(goat_tilde *x, t_symbol *paramname, t_float fslot, t_float value) {
//     control_parameter *param;
//     int slot;

//     if ((param = goat_tilde_validate_parameter(x, paramname->s_name)) == NULL) return;
//     if ((slot = goat_tilde_validate_slot(fslot)) == -1) return;

//     control_parameter_amount(param, slot, value);
// }

// void goat_tilde_param_attach(goat_tilde *x, __attribute__((unused)) t_symbol *s, int argc, t_atom *argv) {
//     control_parameter *param;
//     control_modulator *mod;
//     int slot;

//     t_symbol *paramname = atom_getsymbolarg(0, argc, argv);
//     float fslot = atom_getfloatarg(1, argc, argv);
//     t_symbol *modname = atom_getsymbolarg(2, argc, argv);

//     if ((param = goat_tilde_validate_parameter(x, paramname->s_name)) == NULL) return;
//     if ((mod = goat_tilde_validate_modulator(x, modname->s_name)) == NULL) return;
//     if ((slot = goat_tilde_validate_slot(fslot)) == -1) return;

//     if (param->slots[slot].mod == mod) {
//         error("goat~: already attached");
//         return;
//     }

//     control_parameter_attach(param, slot, mod);
// }

// void goat_tilde_param_detach(goat_tilde *x, t_symbol *paramname, t_floatarg fslot) {
//     control_parameter *param;
//     int slot;

//     if ((param = goat_tilde_validate_parameter(x, paramname->s_name)) == NULL) return;
//     if ((slot = goat_tilde_validate_slot(fslot)) == -1) return;

//     if (param->slots[slot].mod == NULL) {
//         error("goat~: already detached");
//         return;
//     }

//     control_parameter_detach(param, slot);
// }

// void goat_tilde_param_post(goat_tilde *x) {
//     control_parameter *p;
//     int i;

//     post("PARAMETERS:");
//     LL_FOREACH(x->g->cfg.mgr->parameters, p) {
//         startpost("    %s: %.2f <- %.2f",
//             p->name,
//             param(float, p), //value
//             p->offset);
//         for (i = 0; i < CONTROL_NUM_SLOTS; i++) {
//             if (p->slots[i].mod) {
//                 startpost(" + %s * %.2f",
//                     p->slots[i].mod->name,
//                     p->slots[i].amount);
//             }
//         }
//         endpost();
//     }
// }

// void goat_tilde_param_reset(goat_tilde *x){
//     control_parameter *p;
//     int i;

//     LL_FOREACH(x->g->cfg.mgr->parameters, p) {
//         p->offset=p->reset;
//         p->value=p->reset;
//         for (i = 0; i < CONTROL_NUM_SLOTS; i++) {
//             if (p->slots[i].mod) {
//                 control_parameter_amount(p,i,1.0f);
//                 control_parameter_detach(p,i);
//             }
//         }
//     }
//     // post("DEFAULTS:");
//     // goat_tilde_param_post(x);
// }


#ifdef __cplusplus
}
#endif



