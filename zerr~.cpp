#include "zerr_tilde.h"
#include "zerr.h"

#ifdef __cplusplus
extern "C" {
#endif

static t_class *zerr_tilde_class;

void *zerr_tilde_new(t_symbol *s, int argc, t_atom *argv) {
    zerr_tilde *x = (zerr_tilde *) pd_new(zerr_tilde_class);
    if (!x) return NULL;

    // sys_config config = {
    //     .sample_rate = (int) sys_getsr(),
    //     .block_size  = (int) sys_getblksize()
    // };

    SystemConfig sys_cnfg((int) sys_getsr(), (int) sys_getblksize());

    // tmp input
    // the input string muss be absolute?
    std::string spkrCfgFile = "/Users/yangzeyu/Downloads/Zerr/configs/spkr_configs/circulation_8.yaml";
    // std::string zerrCfgFile = "./configs/zerr_configs/preset1.yaml";

    x->z = new Zerr(sys_cnfg, spkrCfgFile);
    if (!x->z) return NULL;
    x->z->initialize();

    x->x_n = x->z->n_outlet; 
    x->x_vec = (t_zerrout *)getbytes(x->x_n * sizeof(*x->x_vec));

    t_zerrout *u;
    int i;

    for (i = 0, u = x->x_vec; i < x->x_n; u++, i++){
        u->u_outlet = outlet_new(&x->x_obj, &s_signal);
    }

    return (void *) x;
}

void zerr_tilde_free(zerr_tilde *x) {
    freebytes(x->x_vec, x->x_n * sizeof(*x->x_vec));
    delete x->z;
}

static t_int *zerr_tilde_perform(t_int *w) {
    zerr_tilde *x = (zerr_tilde *) w[1];
    t_sample *in = (t_sample *) w[2];
    t_sample *out = (t_sample *) w[3];
    int n = (int) w[4];

    // invoke the main algorithm
    // zerr_perform(x->z, in, out, n);
    x->z->perform(in, out, n);

    return &w[5];
}

void zerr_tilde_dsp(zerr_tilde *x, t_signal **sp) {
    t_sample *in = sp[0]->s_vec;
    t_sample *out = sp[1]->s_vec;
    t_sample *out3 = sp[2]->s_vec;
    int n = sp[0]->s_n;

    dsp_add(zerr_tilde_perform, 4, x, in, out, n);
}

void zerr_tilde_setup(void) {
    zerr_tilde_class = class_new(gensym("zerr~"),
        (t_newmethod) zerr_tilde_new,
        (t_method) zerr_tilde_free,
        (size_t) sizeof(zerr_tilde),
        CLASS_DEFAULT,
        A_NULL);

    // class_addmethod(goat_tilde_class,
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

    class_addmethod(zerr_tilde_class,
        (t_method) zerr_tilde_dsp,
        gensym("dsp"),
        A_CANT,
        A_NULL);

    // class_sethelpsymbol(zerr_tilde_class, gensym("zerr~"));
    CLASS_MAINSIGNALIN(zerr_tilde_class, zerr_tilde, f);
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



