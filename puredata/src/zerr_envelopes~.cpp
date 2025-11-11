/**
 * @file zerr_envelopes~.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Implementation of the zerr_envelopes~ object for Pure Data.
 *        This external allows for dynamic spatial audio manipulation within Pure Data,
 *        providing functionalities such as speaker activation, trajectory following, and more.
 * @date 2024-01-30
 *
 * @copyright Copyright (c) 2023-2025
 */

#include "./zerr_envelopes_tilde.h"

#ifdef __cplusplus
extern "C" {
#endif

// Declaration of the class for the zerr_envelopes~ object
static t_class* zerr_envelopes_tilde_class;

/**
 * @brief Constructor for the zerr_envelopes~ object.
 *        Initializes the object with system configuration, analysis arguments,
 *        and sets up inlets and outlets based on provided attributes.
 * @param s Unused symbol parameter, typical in Pd externals.
 * @param argc Count of additional arguments passed during object creation.
 * @param argv Array of t_atom representing the additional arguments.
 * @return void* Pointer to the newly created zerr_envelopes_tilde object.
 */
void* zerr_envelopes_tilde_new(__attribute__((unused)) t_symbol* s, int argc, t_atom* argv)
{
    zerr_envelopes_tilde* x = (zerr_envelopes_tilde*)pd_new(zerr_envelopes_tilde_class);
    if (!x)
        return NULL;

    zerr::SystemConfigs systemCfgs;
    systemCfgs.sample_rate = (size_t)sys_getsr();
    systemCfgs.block_size  = (size_t)sys_getblksize();

    // analysis arguments
    if (argc < 2)
        return NULL; // no enough args to initialize object
    if (argv[0].a_type != A_SYMBOL)
        return NULL;
    char* selectionMode = strdup(atom_getsymbol(argv)->s_name);
    if (argv[1].a_type != A_SYMBOL)
        return NULL;
    char* spkrCfgName = strdup(atom_getsymbol(argv + 1)->s_name);

    // find the absolute path of config file
    t_canvas* canvas = canvas_getcurrent();

    const char* dir;
    dir = canvas_getdir(canvas)->s_name;

    char dirResult[MAXPDSTRING], *nameResult;
    if ((canvas_open(canvas, spkrCfgName, ".yaml", dirResult, &nameResult, MAXPDSTRING, 0)) < 0) {
        pd_error(0, "%s: can't open", spkrCfgName);
        return NULL;
    }

    char spkrCfgFile[MAXPDSTRING] = "";
    strcat(spkrCfgFile, dirResult);
#ifdef WINDOWS
    strcat(spkrCfgFile, "\\");
#else
    strcat(spkrCfgFile, "/");
#endif // WINDOWS
    strcat(spkrCfgFile, nameResult);
    post(spkrCfgFile);

    // create & initialize ZerrEnvelopes object
    x->z = new ZerrEnvelopes(systemCfgs, selectionMode, spkrCfgFile);
    if (!x->z)
        return NULL;
    if (!x->z->initialize())
        return NULL;

    // create inlets
    x->spread_inlet = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
    x->volume_inlet = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);

    // create outlets
    x->n_outlet = x->z->numOutlet; // get the number of outlets
    x->x_vec    = (t_zerrout*)getbytes(x->n_outlet * sizeof(*x->x_vec));
    t_zerrout* u;
    int i;
    for (i = 0, u = x->x_vec; i < x->n_outlet; u++, i++) {
        u->u_outlet = outlet_new(&x->x_obj, &s_signal);
    }

    return (void*)x;
}

/**
 * @brief Destructor for the zerr_envelopes~ object.
 *        Cleans up resources allocated during object creation.
 * @param x Pointer to the zerr_envelopes_tilde object to be freed.
 */
void zerr_envelopes_tilde_free(zerr_envelopes_tilde* x)
{
    freebytes(x->x_vec, x->n_outlet * sizeof(*x->x_vec));
    delete x->z;
}

/**
 * @brief The perform method for the zerr_envelopes~ object.
 *        This is where the audio processing happens.
 * @param w Pointer to an array containing the object instance and audio vectors.
 * @return t_int* Pointer to the next element in the DSP chain.
 */
static t_int* zerr_envelopes_tilde_perform(t_int* w)
{
    zerr_envelopes_tilde* x = (zerr_envelopes_tilde*)w[1];
    int n_vec               = (int)w[2];
    int n_args              = (int)w[3];

    t_sample** ports = (t_sample**)&w[4];

    x->z->perform(ports, n_vec);

    return &w[n_args + 1];
}

/**
 * @brief Method to dynamically adjust active speakers.
 * @param x Pointer to the zerr_envelopes_tilde object.
 * @param s Unused symbol parameter.
 * @param argc Count of arguments passed.
 * @param argv Array of t_atom representing the arguments.
 */
void zerr_envelopes_tilde_active_speakers(zerr_envelopes_tilde* x,
                                          __attribute__((unused)) t_symbol* s, int argc,
                                          t_atom* argv)
{

    if (argc < 2) {
        pd_error(x, "zerr_envelopes~: not enough args to parse");
        return;
    }

    if (argv[0].a_type != A_SYMBOL) {
        pd_error(x, "zerr_envelopes~: no mask action given");
        return;
    }

    char* action = strdup(atom_getsymbol(argv)->s_name);

    int idx_size     = argc - 1;
    int* indexs_list = (int*)getbytes(idx_size * sizeof(int));

    for (int i = 0; i < idx_size; ++i) {
        if (argv[i + 1].a_type != A_FLOAT) {
            pd_error(x, "zerr_envelopes~: incorrect index number");
            return;
        }
        indexs_list[i] = (int)argv[i + 1].a_w.w_float;
    }

    x->z->setActiveSpeakerIndexs(action, indexs_list, argc - 1);
}

/**
 * @brief Method to set the current active speaker.
 * @param x Pointer to the zerr_envelopes_tilde object.
 * @param s Unused symbol parameter.
 * @param argc Count of arguments passed.
 * @param argv Array of t_atom representing the arguments.
 */
void zerr_envelopes_tilde_current_speaker(zerr_envelopes_tilde* x,
                                          __attribute__((unused)) t_symbol* s, int argc,
                                          t_atom* argv)
{

    if (argc < 1) {
        pd_error(x, "zerr_envelopes~: not enough args to parse");
        return;
    }

    int speaker_index = (int)argv[0].a_w.w_float;

    x->z->setCurrentSpeaker(speaker_index);
}

/**
 * @brief Method to adjust the topological matrix of speakers.
 * @param x Pointer to the zerr_envelopes_tilde object.
 * @param s Unused symbol parameter.
 * @param argc Count of arguments passed.
 * @param argv Array of t_atom representing the arguments.
 */
void zerr_envelopes_tilde_topomatrix(zerr_envelopes_tilde* x, __attribute__((unused)) t_symbol* s,
                                     int argc, t_atom* argv)
{

    if (argc < 3) {
        pd_error(x, "zerr_envelopes~: not enough args to parse");
        return;
    }

    if (argv[0].a_type != A_SYMBOL) {
        pd_error(x, "zerr_envelopes~: no action given");
        return;
    }

    char* action = strdup(atom_getsymbol(argv)->s_name);

    int idx_size     = argc - 1;
    int* indexs_list = (int*)getbytes(idx_size * sizeof(int));

    for (int i = 0; i < idx_size; ++i) {
        if (argv[i + 1].a_type != A_FLOAT) {
            pd_error(x, "zerr_envelopes~: incorrect index number");
            return;
        }
        indexs_list[i] = (int)argv[i + 1].a_w.w_float;
    }

    x->z->setTopoMatrix(action, indexs_list, idx_size);
}

/**
 * @brief Method to set the trajectory of sound movement among speakers.
 * @param x Pointer to the zerr_envelopes_tilde object.
 * @param s Unused symbol parameter.
 * @param argc Count of arguments passed.
 * @param argv Array of t_atom representing the arguments.
 */
void zerr_envelopes_tilde_trajectory(zerr_envelopes_tilde* x, __attribute__((unused)) t_symbol* s,
                                     int argc, t_atom* argv)
{
    if (argc < 2) {
        pd_error(x, "zerr_envelopes~: not enough args");
        return;
    }

    int idx_size     = argc;
    int* indexs_list = (int*)getbytes(idx_size * sizeof(int));

    for (int i = 0; i < idx_size; ++i) {
        if (argv[i].a_type != A_FLOAT) {
            pd_error(x, "zerr_envelopes~: incorrect index number");
            return;
        }
        indexs_list[i] = (int)argv[i].a_w.w_float;
    }

    x->z->setTrajectoryVector(indexs_list, idx_size);
}

/**
 * @brief TODO
 * @param x Pointer to the zerr_envelopes_tilde object.
 * @param s Unused symbol parameter.
 * @param argc Count of arguments passed.
 * @param argv Array of t_atom representing the arguments.
 */
void zerr_envelopes_tilde_trigger_interval(zerr_envelopes_tilde* x,
                                           __attribute__((unused)) t_symbol* s, int argc,
                                           t_atom* argv)
{

    if (argc < 1) {
        pd_error(x, "zerr_envelopes~: not enough args to parse");
        return;
    }

    float interval = argv[0].a_w.w_float; // ms

    x->z->setTriggerInterval(interval);
}

/**
 * @brief Method to print the current configuration parameters of the object.
 * @param x Pointer to the zerr_envelopes_tilde object.
 * @param s Unused symbol parameter.
 */
void zerr_envelopes_tilde_print(zerr_envelopes_tilde* x, t_symbol* s) { x->z->printParameters(); }

/**
 * @brief Adds the zerr_envelopes~ object to the DSP chain.
 * @param x Pointer to the zerr_envelopes_tilde object.
 * @param sp Array of t_signal pointers representing the incoming signals.
 */
void zerr_envelopes_tilde_dsp(zerr_envelopes_tilde* x, t_signal** sp)
{
    int n_rest = 3; // size of [x, n_vec, n_args]

    int n_vec  = sp[0]->s_n;
    int n_port = x->z->get_port_count();
    int n_args = n_port + n_rest;

    t_int* vec = (t_int*)getbytes(n_args * sizeof(t_int*));

    vec[0] = (t_int)x;
    vec[1] = (t_int)n_vec;
    vec[2] = (t_int)n_args;
    for (int i = 0; i < n_port; ++i) {
        vec[i + n_rest] = (t_int)sp[i]->s_vec;
    }

    dsp_addv(zerr_envelopes_tilde_perform, n_args, vec);
}

/**
 * @brief Setup function for the zerr_envelopes~ object.
 *        Registers the object with Pure Data, defining its constructor, destructor, and DSP method.
 */
void zerr_envelopes_tilde_setup(void)
{
    zerr_envelopes_tilde_class =
        class_new(gensym("zerr_envelopes~"), (t_newmethod)zerr_envelopes_tilde_new,
                  (t_method)zerr_envelopes_tilde_free, (size_t)sizeof(zerr_envelopes_tilde),
                  CLASS_DEFAULT, A_GIMME, 0);

    class_addmethod(zerr_envelopes_tilde_class, (t_method)zerr_envelopes_tilde_active_speakers,
                    gensym("active"), A_GIMME, A_NULL);

    class_addmethod(zerr_envelopes_tilde_class, (t_method)zerr_envelopes_tilde_current_speaker,
                    gensym("curr"), A_GIMME, A_NULL);

    class_addmethod(zerr_envelopes_tilde_class, (t_method)zerr_envelopes_tilde_topomatrix,
                    gensym("topo"), A_GIMME, A_NULL);

    class_addmethod(zerr_envelopes_tilde_class, (t_method)zerr_envelopes_tilde_trajectory,
                    gensym("traj"), A_GIMME, A_NULL);

    class_addmethod(zerr_envelopes_tilde_class, (t_method)zerr_envelopes_tilde_trigger_interval,
                    gensym("interval"), A_GIMME, A_NULL);

    class_addmethod(zerr_envelopes_tilde_class, (t_method)zerr_envelopes_tilde_print,
                    gensym("print"), A_GIMME, A_NULL);

    class_addmethod(zerr_envelopes_tilde_class, (t_method)zerr_envelopes_tilde_dsp, gensym("dsp"),
                    A_CANT, A_NULL);

    class_sethelpsymbol(zerr_envelopes_tilde_class, gensym("zerr_envelopes~"));
    CLASS_MAINSIGNALIN(zerr_envelopes_tilde_class, zerr_envelopes_tilde, f);
}

#ifdef __cplusplus
}
#endif
