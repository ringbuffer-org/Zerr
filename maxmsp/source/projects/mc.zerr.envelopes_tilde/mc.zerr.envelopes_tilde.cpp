/**
 * @file    mc.zerr.envelopes_tilde.cpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   mc.zerr.envelopes~ Max/MSP External using Max API for better multi-channel support
 * @date    2025-05-03
 *
 * @copyright  Copyright (c) 2023-2025
 * @license    MIT license
 */

#include "commonsyms.h"
#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

#include "./zerr_envelopes.hpp"

typedef struct _zerr_envelopes {
    t_pxobject x_obj; // DSP object header
    long channel_count; // Channel count for output
    ZerrEnvelopes* ze; // Pointer to the zerr_envelopes implementation from zerr_core
} t_zerr_envelopes;

void* zerr_envelopes_new(t_symbol* s, long argc, t_atom* argv);
void zerr_envelopes_free(t_zerr_envelopes* x);
void zerr_envelopes_assist(t_zerr_envelopes* x, void* b, long m, long a, char* s);
void zerr_envelopes_dsp64(t_zerr_envelopes* x, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags);
void zerr_envelopes_perform64(t_zerr_envelopes* x, t_object* dsp64, double** ins, long numins, double** outs, long numouts, long sampleframes, long flags, void* userparam);
long zerr_envelopes_multichanneloutputs(t_zerr_envelopes* x, long outletindex);
//--------------------------------------------------------------------------
void zerr_envelopes_bang(t_zerr_envelopes* x);
void zerr_envelopes_active(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv);
void zerr_envelopes_curr(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv);
void zerr_envelopes_topo(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv);
void zerr_envelopes_traj(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv);
void zerr_envelopes_interval(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv);
void zerr_envelopes_print(t_zerr_envelopes* x);

static t_class* zerr_envelopes_class = NULL;

//--------------------------------------------------------------------------

C74_EXPORT void ext_main(void* r)
{
    t_class* c;

    c = class_new("mc.zerr.envelopes~",
        (method)zerr_envelopes_new,
        (method)zerr_envelopes_free,
        sizeof(t_zerr_envelopes),
        0L,
        A_GIMME,
        0);

    class_addmethod(c, (method)zerr_envelopes_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)zerr_envelopes_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)zerr_envelopes_multichanneloutputs, "multichanneloutputs", A_CANT, 0);
    // Messages
    class_addmethod(c, (method)zerr_envelopes_bang, "bang", 0);
    class_addmethod(c, (method)zerr_envelopes_active, "active", A_GIMME, 0);
    class_addmethod(c, (method)zerr_envelopes_curr, "curr", A_GIMME, 0);
    class_addmethod(c, (method)zerr_envelopes_topo, "topo", A_GIMME, 0);
    class_addmethod(c, (method)zerr_envelopes_traj, "traj", A_GIMME, 0);
    class_addmethod(c, (method)zerr_envelopes_interval, "interval", A_GIMME, 0);
    class_addmethod(c, (method)zerr_envelopes_print, "print", 0);

    CLASS_ATTR_LONG(c, "chans", 0, t_zerr_envelopes, channel_count);
    CLASS_ATTR_LABEL(c, "chans", 0, "Output Channels");
    CLASS_ATTR_FILTER_CLIP(c, "chans", 1, MC_MAX_CHANS);
    CLASS_ATTR_BASIC(c, "chans", 0);

    class_dspinit(c);
    class_register(CLASS_BOX, c);
    zerr_envelopes_class = c;
}

//--------------------------------------------------------------------------

void* zerr_envelopes_new(t_symbol* s, long argc, t_atom* argv)
{
    t_zerr_envelopes* x = (t_zerr_envelopes*)object_alloc(zerr_envelopes_class);

    if (x) {
        x->ze = NULL;
        x->channel_count = 1; // Default 1 channel
        bool yaml_found = false;

        char* full_path = NULL;
        char* unix_path = NULL;
        char* colon_pos = NULL;

        // Only two arguments required
        if (argc != 2) {
            return NULL;
        }

        // Process argument 1: mode
        t_symbol* arg1 = atom_getsym(argv);
        const char* mode = arg1->s_name;
        post("envelopes mode: %s", mode);

        // Process argument 2: config file path
        t_symbol* arg2 = atom_getsym(argv + 1);
        const char* path = arg2->s_name;
        size_t len = strlen(path);
        post("Configuration file path: %s", path);

        t_symbol* absolute_path = NULL;
        t_fourcc file_types[] = { 'TEXT', 'YAML' }; // Try both types
        t_max_err err;

        if ((len > 5 && strcmp(path + len - 5, ".yaml") == 0) || (len > 4 && strcmp(path + len - 4, ".yml") == 0)) {
            post("Configuration file path: ended with .yaml or .yml");
            err = path_absolutepath(&absolute_path, arg2, file_types, 2);

            if (err == MAX_ERR_NONE && absolute_path) {
                full_path = absolute_path->s_name;
                unix_path = full_path;

                // For Linux & MacOS
                colon_pos = strchr(full_path, ':');
                if (colon_pos) {
                    unix_path = colon_pos + 1;
                }

                post("Found YAML file at: %s", unix_path);
                yaml_found = true;
            }
        } else {
            post("Search with additional .yaml|.yml suffix");
            err = path_absolutepath(&absolute_path, arg2, file_types, 2);

            // Try with .yaml first
            char yaml_name[MAX_PATH_CHARS];
            snprintf(yaml_name, MAX_PATH_CHARS, "%s.yaml", path);
            t_symbol* yaml_sym = gensym(yaml_name);

            err = path_absolutepath(&absolute_path, yaml_sym, file_types, 2);
            if (err == MAX_ERR_NONE && absolute_path) {

                full_path = absolute_path->s_name;
                unix_path = full_path;

                // For Linux & MacOS
                colon_pos = strchr(full_path, ':');
                if (colon_pos) {
                    unix_path = colon_pos + 1;
                }

                post("Found YAML file at: %s", unix_path);

                yaml_found = true;
            } else {
                // Try with .yml
                char yml_name[MAX_PATH_CHARS];
                snprintf(yml_name, MAX_PATH_CHARS, "%s.yml", path);
                t_symbol* yml_sym = gensym(yml_name);

                err = path_absolutepath(&absolute_path, yml_sym, file_types, 2);
                if (err == MAX_ERR_NONE && absolute_path) {
                    full_path = absolute_path->s_name;
                    unix_path = full_path;

                    // For Linux & MacOS
                    colon_pos = strchr(full_path, ':');
                    if (colon_pos) {
                        unix_path = colon_pos + 1;
                    }

                    post("Found YAML file at: %s", unix_path);
                    yaml_found = true;
                } else {
                    error("Cannot find YAML file: %s (tried as-is, with .yaml, and with .yml)", path);
                }
            }
        }

        if (!yaml_found) {
            error("Cannot find YAML file: %s", arg2->s_name);
            return NULL;
        }

        // zerr data structure for saving feature names
        zerr::t_featureNames ft_names;
        ft_names.names = (char**)malloc(argc * sizeof(char*));
        ft_names.num = argc;

        // copy arguments to ft_names structure
        for (int i = 0; i < argc; i++) {
            ft_names.names[i] = strdup(atom_getsym(argv + i)->s_name);
        }

        // system config to initialize zerr: sample rate, block size
        zerr::SystemConfigs sys_cnfg;
        sys_cnfg.sample_rate = (size_t)sys_getsr();
        sys_cnfg.block_size = (size_t)sys_getblksize();

        // create & initialize ZerrEnvelopes instance
        x->ze = new ZerrEnvelopes(sys_cnfg, mode, unix_path);
        if (!x->ze)
            return NULL;

        if (!x->ze->initialize())
            return NULL;

        // Ensure channel count is within bounds
        x->channel_count = x->ze->getOutputCount();

        // Process attributes first
        attr_args_process(x, argc, argv);

        // Set up DSP
        dsp_setup((t_pxobject*)x, x->ze->getInputCount());

        // Create multichannel outlet
        outlet_new((t_object*)x, "multichannelsignal");
    }

    return x;
}

//--------------------------------------------------------------------------

void zerr_envelopes_free(t_zerr_envelopes* x)
{
    dsp_free((t_pxobject*)x);
    if (x->ze) {
        delete x->ze;
    }
}

//--------------------------------------------------------------------------

void zerr_envelopes_assist(t_zerr_envelopes* x, void* b, long m, long a, char* s)
{
    if (m == ASSIST_INLET) {
        strcpy(s, "(signal) Input source signal");
    } else if (m == ASSIST_OUTLET) {
        strcpy(s, "(multichannel signal) Output envelopes");
    }
}

//--------------------------------------------------------------------------

long zerr_envelopes_multichanneloutputs(t_zerr_envelopes* x, long outletindex)
{
    return x->channel_count;
}

//--------------------------------------------------------------------------

void zerr_envelopes_dsp64(t_zerr_envelopes* x, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags)
{
    dsp_add64(dsp64, (t_object*)x, (t_perfroutine64)zerr_envelopes_perform64, 0, NULL);
    post("zerr_envelopes_dsp64: samplerate %.0lf maxvectorsize %d", samplerate, maxvectorsize);
}

//--------------------------------------------------------------------------

void zerr_envelopes_perform64(t_zerr_envelopes* x, t_object* dsp64, double** ins, long numins, double** outs, long numouts, long sampleframes, long flags, void* userparam)
{
    x->ze->perform(ins, numins, outs, numouts, sampleframes);
}

//--------------------------------------------------------------------------

void zerr_envelopes_bang(t_zerr_envelopes* x)
{
    post("mc.zerr.envelopes~: current channel count = %ld", x->channel_count);
}

void zerr_envelopes_active(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv)
{
    t_symbol* action;
    long i, index_count;
    int* indices;

    if (argc < 2) {
        object_error((t_object*)x, "active: not enough arguments");
        return;
    }

    if (atom_gettype(argv) != A_SYM) {
        object_error((t_object*)x, "active: first argument must be a symbol");
        return;
    }

    action = atom_getsym(argv);
    index_count = argc - 1;

    indices = (int*)sysmem_newptr(index_count * sizeof(int));
    if (!indices) {
        object_error((t_object*)x, "active: out of memory");
        return;
    }

    for (i = 0; i < index_count; i++) {
        if (atom_gettype(argv + i + 1) == A_LONG) {
            indices[i] = (int)atom_getlong(argv + i + 1);
        } else if (atom_gettype(argv + i + 1) == A_FLOAT) {
            indices[i] = (int)atom_getfloat(argv + i + 1);
        } else {
            object_error((t_object*)x, "active: index %ld must be a number", i + 1);
            sysmem_freeptr(indices);
            return;
        }
    }

    x->ze->setActiveSpeakerIndexs(action->s_name, indices, index_count);

    sysmem_freeptr(indices);
}

// void zerr_envelopes_curr(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv)
// {
//     post("zerr_envelopes_curr");
// }

// void zerr_envelopes_topo(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv)
// {
//     post("zerr_envelopes_topo");
// }
// void zerr_envelopes_traj(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv)
// {
//     post("zerr_envelopes_traj");
// }

// void zerr_envelopes_interval(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv)
// {
//     post("zerr_envelopes_interval");
// }

// void zerr_envelopes_print(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv)
// {
//     post("zerr_envelopes_print");
// }

/**
 * @brief Method to set the current active speaker.
 * @param x Pointer to the t_zerr_envelopes object.
 * @param s Unused symbol parameter.
 * @param argc Count of arguments passed.
 * @param argv Array of t_atom representing the arguments.
 */
void zerr_envelopes_curr(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv)
{
    long speaker_index;

    if (argc < 1) {
        object_error((t_object*)x, "current_speaker: not enough arguments");
        return;
    }

    if (atom_gettype(argv) == A_LONG) {
        speaker_index = atom_getlong(argv);
    } else if (atom_gettype(argv) == A_FLOAT) {
        speaker_index = (long)atom_getfloat(argv);
    } else {
        object_error((t_object*)x, "current_speaker: argument must be a number");
        return;
    }

    x->ze->setCurrentSpeaker((int)speaker_index);
}

/**
 * @brief Method to adjust the topological matrix of speakers.
 * @param x Pointer to the t_zerr_envelopes object.
 * @param s Unused symbol parameter.
 * @param argc Count of arguments passed.
 * @param argv Array of t_atom representing the arguments.
 */
void zerr_envelopes_topo(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv)
{
    t_symbol* action;
    long i, index_count;
    int* indices;

    if (argc < 3) {
        object_error((t_object*)x, "topomatrix: not enough arguments");
        return;
    }

    if (atom_gettype(argv) != A_SYM) {
        object_error((t_object*)x, "topomatrix: first argument must be a symbol");
        return;
    }

    action = atom_getsym(argv);
    index_count = argc - 1;

    indices = (int*)sysmem_newptr(index_count * sizeof(int));
    if (!indices) {
        object_error((t_object*)x, "topomatrix: out of memory");
        return;
    }

    for (i = 0; i < index_count; i++) {
        if (atom_gettype(argv + i + 1) == A_LONG) {
            indices[i] = (int)atom_getlong(argv + i + 1);
        } else if (atom_gettype(argv + i + 1) == A_FLOAT) {
            indices[i] = (int)atom_getfloat(argv + i + 1);
        } else {
            object_error((t_object*)x, "topomatrix: index %ld must be a number", i + 1);
            sysmem_freeptr(indices);
            return;
        }
    }

    x->ze->setTopoMatrix(action->s_name, indices, (int)index_count);

    sysmem_freeptr(indices);
}

/**
 * @brief Method to set the trajectory of sound movement among speakers.
 * @param x Pointer to the t_zerr_envelopes object.
 * @param s Unused symbol parameter.
 * @param argc Count of arguments passed.
 * @param argv Array of t_atom representing the arguments.
 */
void zerr_envelopes_traj(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv)
{
    long i;
    int* indices;

    if (argc < 2) {
        object_error((t_object*)x, "trajectory: not enough arguments");
        return;
    }

    indices = (int*)sysmem_newptr(argc * sizeof(int));
    if (!indices) {
        object_error((t_object*)x, "trajectory: out of memory");
        return;
    }

    for (i = 0; i < argc; i++) {
        if (atom_gettype(argv + i) == A_LONG) {
            indices[i] = (int)atom_getlong(argv + i);
        } else if (atom_gettype(argv + i) == A_FLOAT) {
            indices[i] = (int)atom_getfloat(argv + i);
        } else {
            object_error((t_object*)x, "trajectory: index %ld must be a number", i);
            sysmem_freeptr(indices);
            return;
        }
    }

    x->ze->setTrajectoryVector(indices, (int)argc);

    sysmem_freeptr(indices);
}

/**
 * @brief Method to set the trigger interval
 * @param x Pointer to the t_zerr_envelopes object.
 * @param s Unused symbol parameter.
 * @param argc Count of arguments passed.
 * @param argv Array of t_atom representing the arguments.
 */
void zerr_envelopes_interval(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv)
{
    double interval;

    if (argc < 1) {
        object_error((t_object*)x, "trigger_interval: not enough arguments");
        return;
    }

    if (atom_gettype(argv) == A_FLOAT) {
        interval = atom_getfloat(argv);
    } else if (atom_gettype(argv) == A_LONG) {
        interval = (double)atom_getlong(argv);
    } else {
        object_error((t_object*)x, "trigger_interval: argument must be a number");
        return;
    }

    x->ze->setTriggerInterval((float)interval);
}

/**
 * @brief Method to print the current configuration parameters of the object.
 * @param x Pointer to the t_zerr_envelopes object.
 * @param s Unused symbol parameter.
 */
void zerr_envelopes_print(t_zerr_envelopes* x)
{
    x->ze->printParameters();
}
