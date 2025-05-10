/**
 * @file    mc.zerr.features_tilde.cpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   zerr.features~ Max/MSP External
 * @date    2025-05-02
 *
 * @copyright  Copyright (c) 2023-2025
 * @license    MIT license
 */

#include "commonsyms.h"
#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

#include "./zerr_features.hpp"

typedef struct _zerr_features {
    t_pxobject x_obj; // DSP object header
    long channel_count; // Channel count for output
    ZerrFeatures *zf; // Pointer to the zerr_features implementation from zerr_core
} t_zerr_features;

void* zerr_features_new(t_symbol* s, long argc, t_atom* argv);
void zerr_features_free(t_zerr_features* x);
void zerr_features_assist(t_zerr_features* x, void* b, long m, long a, char* s);
void zerr_features_dsp64(t_zerr_features* x, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags);
void zerr_features_perform64(t_zerr_features* x, t_object* dsp64, double** ins, long numins, double** outs, long numouts, long sampleframes, long flags, void* userparam);
void zerr_features_bang(t_zerr_features* x);

static t_class* zerr_features_class = NULL;

//--------------------------------------------------------------------------

C74_EXPORT void ext_main(void* r)
{
    t_class* c;

    c = class_new("zerr.features~",
        (method)zerr_features_new,
        (method)zerr_features_free,
        sizeof(t_zerr_features),
        0L,
        A_GIMME,
        0);

    class_addmethod(c, (method)zerr_features_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)zerr_features_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)zerr_features_bang, "bang", 0);

    CLASS_ATTR_LONG(c, "chans", 0, t_zerr_features, channel_count);
    CLASS_ATTR_LABEL(c, "chans", 0, "Output Channels");
    CLASS_ATTR_FILTER_CLIP(c, "chans", 1, MC_MAX_CHANS);
    CLASS_ATTR_BASIC(c, "chans", 0);

    class_dspinit(c);
    class_register(CLASS_BOX, c);
    zerr_features_class = c;
}

//--------------------------------------------------------------------------

void* zerr_features_new(t_symbol* s, long argc, t_atom* argv)
{
    t_zerr_features* x = (t_zerr_features*)object_alloc(zerr_features_class);
    
    if (x) {
        // Initialize to default values
        x->zf = NULL;
        x->channel_count = 1; // Default 1 channel

        // Process arguments
        long offset = attr_args_offset(argc, argv);
        if (offset >= 1) {
            x->channel_count = offset; // the number of argument defines the channel
        }

        // Ensure channel count is within bounds
        x->channel_count = CLAMP(x->channel_count, 1, MC_MAX_CHANS);

        // zerr data structure for saving feature names
        zerr::t_featureNames ft_names;
        ft_names.names = NULL;
        ft_names.num = 0;
        
        if (argc > 0) {
            ft_names.names = (char**)malloc(argc * sizeof(char*));
            if (!ft_names.names) {
                object_error((t_object*)x, "Failed to allocate memory for feature names");
                object_free(x);
                return NULL;
            }
            
            ft_names.num = argc;

            // copy arguments to ft_names structure
            for (int i = 0; i < argc; i++) {
                ft_names.names[i] = strdup(atom_getsym(argv + i)->s_name);
                if (!ft_names.names[i]) {
                    // Clean up previously allocated strings
                    for (int j = 0; j < i; j++) {
                        free(ft_names.names[j]);
                    }
                    free(ft_names.names);
                    object_error((t_object*)x, "Failed to allocate memory for feature name");
                    object_free(x);
                    return NULL;
                }
            }
        }

        // system config to initialize zerr: sample rate, block size
        zerr::SystemConfigs sys_cnfg;
        sys_cnfg.sample_rate = (size_t)sys_getsr();
        sys_cnfg.block_size = (size_t)sys_getblksize();

        // Create & initialize ZerrFeatures instance
        try {
            x->zf = new ZerrFeatures(sys_cnfg, ft_names);
            
            if (!x->zf->initialize()) {
                delete x->zf;
                x->zf = NULL;
                
                // Clean up feature names
                for (int i = 0; i < ft_names.num; i++) {
                    free(ft_names.names[i]);
                }
                free(ft_names.names);
                
                object_error((t_object*)x, "Failed to initialize ZerrFeatures");
                object_free(x);
                return NULL;
            }
        }
        catch (...) {
            // Clean up feature names
            for (int i = 0; i < ft_names.num; i++) {
                free(ft_names.names[i]);
            }
            free(ft_names.names);
            
            object_error((t_object*)x, "Exception while creating ZerrFeatures");
            object_free(x);
            return NULL;
        }

        // Clean up feature names after successful initialization
        for (int i = 0; i < ft_names.num; i++) {
            free(ft_names.names[i]);
        }
        free(ft_names.names);

        // Set up DSP with 1 inlet (not channel_count)
        dsp_setup((t_pxobject*)x, 1); // One signal inlet

        // Create outlets
        for (int i = 0; i < x->channel_count; ++i) {
            outlet_new((t_object*)x, "signal");
        }

        // Process attribute args
        attr_args_process(x, argc, argv);
    }

    return x;
}

//--------------------------------------------------------------------------

void zerr_features_free(t_zerr_features* x)
{
    dsp_free((t_pxobject*)x);
    
    if (x->zf) {
        delete x->zf;
        x->zf = NULL;
    }
}

//--------------------------------------------------------------------------

void zerr_features_assist(t_zerr_features* x, void* b, long m, long a, char* s)
{
    if (m == ASSIST_INLET) {
        strcpy(s, "(signal) Input source signal");
    } else if (m == ASSIST_OUTLET) {
        sprintf(s, "(signal) Feature channel %ld", a + 1);
    }
}

//--------------------------------------------------------------------------

void zerr_features_dsp64(t_zerr_features* x, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags)
{
    dsp_add64(dsp64, (t_object*)x, (t_perfroutine64)zerr_features_perform64, 0, NULL);
    post("zerr_features_dsp64: samplerate %.0lf maxvectorsize %ld", samplerate, maxvectorsize);
}

//--------------------------------------------------------------------------

void zerr_features_perform64(t_zerr_features* x, t_object* dsp64, double** ins, long numins, double** outs, long numouts, long sampleframes, long flags, void* userparam)
{
    // Add safety checks
    if (!x || !x->zf) {
        // Clear all outputs if we have an invalid state
        for (long i = 0; i < numouts; i++) {
            if (outs[i]) {
                memset(outs[i], 0, sampleframes * sizeof(double));
            }
        }
        return;
    }
    
    // Validate buffer pointers
    if (!ins || !outs) {
        return;
    }
    
    // Perform the DSP processing
    x->zf->perform(ins, numins, outs, numouts, sampleframes);
}

//--------------------------------------------------------------------------

void zerr_features_bang(t_zerr_features* x)
{
    post("zerr.features~: current channel count = %ld", x->channel_count);
}