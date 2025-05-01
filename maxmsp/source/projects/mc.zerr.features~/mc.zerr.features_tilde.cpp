/**
 * @file    mc.zerr.features_tilde.c
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   mc.zerr.features~ Max/MSP External
 * @date    2025-05-01
 *
 * @copyright  Copyright (c) 2023-2025
 * @license    MIT license
 */

#include "commonsyms.h"
#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

#include "./mc_zerr_features.hpp"

typedef struct _zerr_features {
    t_pxobject x_obj; // DSP object header
    long channel_count; // Channel count for output
    // ZerrFeatures *z; 
} t_zerr_features;

void* zerr_features_new(t_symbol* s, long argc, t_atom* argv);
void zerr_features_free(t_zerr_features* x);
void zerr_features_assist(t_zerr_features* x, void* b, long m, long a, char* s);
void zerr_features_dsp64(t_zerr_features* x, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags);
void zerr_features_perform64(t_zerr_features* x, t_object* dsp64, double** ins, long numins, double** outs, long numouts, long sampleframes, long flags, void* userparam);
long zerr_features_multichanneloutputs(t_zerr_features* x, long outletindex);
void zerr_features_bang(t_zerr_features* x);

static t_class* zerr_features_class = NULL;

//--------------------------------------------------------------------------

void ext_main(void* r)
{
    t_class* c;

    c = class_new("mc.zerr.features~",
        (method)zerr_features_new,
        (method)zerr_features_free,
        sizeof(t_zerr_features),
        0L,
        A_GIMME,
        0);

    class_addmethod(c, (method)zerr_features_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)zerr_features_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)zerr_features_multichanneloutputs, "multichanneloutputs", A_CANT, 0);
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
    long offset;

    if (x) {
        x->channel_count = 1; // Default 1 channel

        offset = attr_args_offset(argc, argv);
        if (offset >= 1) {
            x->channel_count = offset; // the number of argument defines the channel
        }

        // Ensure channel count is within bounds
        x->channel_count = CLAMP(x->channel_count, 1, MC_MAX_CHANS);

        // Allocate memory if needed
        // x->values = (double*)sysmem_newptrclear(x->channel_count * sizeof(double));

        dsp_setup((t_pxobject*)x, 1); // One signal inlet

        // Mark as multichannel enabled
        // x->x_obj.z_misc = Z_NO_INPLACE | Z_MC_INLETS;

        // Create multichannel outlet
        outlet_new((t_object*)x, "multichannelsignal");

        // Process attribute args
        attr_args_process(x, argc, argv);

    }

    return x;
}

//--------------------------------------------------------------------------

void zerr_features_free(t_zerr_features* x)
{
    dsp_free((t_pxobject*)x);

    // if (x->values) {
    //     sysmem_freeptr(x->values);
    //     x->values = NULL;
    // }
}

//--------------------------------------------------------------------------

void zerr_features_assist(t_zerr_features* x, void* b, long m, long a, char* s)
{
    if (m == ASSIST_INLET) {
        strcpy(s, "(signal) Input source signal");
    } else if (m == ASSIST_OUTLET) {
        strcpy(s, "(multichannel signal) Output extracted audio features");
    }
}

//--------------------------------------------------------------------------

long zerr_features_multichanneloutputs(t_zerr_features* x, long outletindex)
{
    return x->channel_count;
}

//--------------------------------------------------------------------------

void zerr_features_dsp64(t_zerr_features* x, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags)
{
    dsp_add64(dsp64, (t_object*)x, (t_perfroutine64)zerr_features_perform64, 0, NULL);
}

//--------------------------------------------------------------------------

void zerr_features_perform64(t_zerr_features* x, t_object* dsp64, double** ins, long numins, double** outs, long numouts, long sampleframes, long flags, void* userparam)
{
    for (long channel = 0; channel < numouts; channel++) {
        if (channel < x->channel_count) {
            memset(outs[channel], 0, sampleframes * sizeof(double));
        }
    }
}

//--------------------------------------------------------------------------

void zerr_features_bang(t_zerr_features* x)
{
    post("mc.zerr.features~: current channel count = %ld", x->channel_count);
}
