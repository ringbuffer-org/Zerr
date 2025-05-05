/**
 * @file    mc.zerr.combinator_tilde.cpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   mc.zerr.combinator~ Max/MSP External using Max API for better multi-channel support
 * @date    2025-05-03
 *
 * @copyright  Copyright (c) 2023-2025
 * @license    MIT license
 */

#include "commonsyms.h"
#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

#include "./zerr_combinator.hpp"

typedef struct _zerr_combinator {
    t_pxobject x_obj; // DSP object header
    long channel_count; // Channel count for output/input
    long input_count;
    bool channel_muted;
    ZerrCombinator* zc; // Pointer to the zerr_combinator implementation from zerr_core
} t_zerr_combinator;

void* zerr_combinator_new(t_symbol* s, long argc, t_atom* argv);
void zerr_combinator_free(t_zerr_combinator* x);
void zerr_combinator_assist(t_zerr_combinator* x, void* b, long m, long a, char* s);
void zerr_combinator_dsp64(t_zerr_combinator* x, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags);
void zerr_combinator_perform64(t_zerr_combinator* x, t_object* dsp64, double** ins, long numins, double** outs, long numouts, long sampleframes, long flags, void* userparam);
// Multi-channel specific functions
long zerr_combinator_multichanneloutputs(t_zerr_combinator* x, long outletindex);
long zerr_combinator_inputchanged(t_zerr_combinator* x, long index, long count);

static t_class* zerr_combinator_class = NULL;

//--------------------------------------------------------------------------

C74_EXPORT void ext_main(void* r)
{
    t_class* c;

    c = class_new("mc.zerr.combinator~",
        (method)zerr_combinator_new,
        (method)zerr_combinator_free,
        sizeof(t_zerr_combinator),
        0L,
        A_GIMME,
        0);

    class_addmethod(c, (method)zerr_combinator_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)zerr_combinator_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)zerr_combinator_multichanneloutputs, "multichanneloutputs", A_CANT, 0);
    class_addmethod(c, (method)zerr_combinator_inputchanged, "inputchanged", A_CANT, 0);

    CLASS_ATTR_LONG(c, "chans", 0, t_zerr_combinator, channel_count);
    CLASS_ATTR_LABEL(c, "chans", 0, "Output Channels");
    CLASS_ATTR_FILTER_CLIP(c, "chans", 1, MC_MAX_CHANS);
    CLASS_ATTR_BASIC(c, "chans", 0);

    class_dspinit(c);
    class_register(CLASS_BOX, c);

    zerr_combinator_class = c;
}

//--------------------------------------------------------------------------

void* zerr_combinator_new(t_symbol* s, long argc, t_atom* argv)
{
    t_zerr_combinator* x = (t_zerr_combinator*)object_alloc(zerr_combinator_class);

    if (x) {
        x->zc = NULL;
        x->channel_count = 1; // Default 1 channel
        x->input_count = 2; // Default combine two sets of envelopes
        x->channel_muted = FALSE;

        // Only two arguments required
        if (argc < 2) {
            object_error(NULL, "mc.zerr.combinator~: requires 2 arguments (input_count mode)");
            return NULL;
        }

        // Process argument 1: number of inputs
        if (atom_gettype(argv) == A_LONG) {
            x->input_count = atom_getlong(argv);
        } else {
            object_error(NULL, "mc.zerr.combinator~: first argument must be an integer");
            return NULL;
        }

        // Process argument 2: combine mode
        if (atom_gettype(argv + 1) != A_SYM) {
            object_error(NULL, "mc.zerr.combinator~: second argument must be a symbol");
            return NULL;
        }
        t_symbol* arg2 = atom_getsym(argv + 1);
        const char* mode = arg2->s_name;

        // Process attributes
        attr_args_process(x, argc, argv);

        // Set up DSP with the number of inlets
        dsp_setup((t_pxobject*)x, x->input_count);
        x->x_obj.z_misc = Z_MC_INLETS;

        // Create multichannel outlet
        outlet_new((t_object*)x, "multichannelsignal");

        post("mc.zerr.combinator~: created with %ld inputs", x->input_count);
    }

    return x;
}

//--------------------------------------------------------------------------

void zerr_combinator_free(t_zerr_combinator* x)
{
    dsp_free((t_pxobject*)x);
    // if (x->zc) {
    //     delete x->zc;
    // }
}

//--------------------------------------------------------------------------

void zerr_combinator_assist(t_zerr_combinator* x, void* b, long m, long a, char* s)
{
    if (m == ASSIST_INLET) {
        strcpy(s, "(signal) Input different set of envelopes ");
    } else if (m == ASSIST_OUTLET) {
        strcpy(s, "(multichannel signal) Output envelopes");
    }
}

//--------------------------------------------------------------------------

long zerr_combinator_multichanneloutputs(t_zerr_combinator* x, long outletindex)
{
    return x->channel_count;
}

long zerr_combinator_inputchanged(t_zerr_combinator* x, long index, long count)
{
    // if (count != x->channel_count) {
    if (index == 0) {
        x->channel_count = count;
        return true;
    } else
        return false;
}

//--------------------------------------------------------------------------

void zerr_combinator_dsp64(t_zerr_combinator* x, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags)
{
    post("zerr_combinator_dsp64: samplerate %.0lf maxvectorsize %d", samplerate, maxvectorsize);

    // Get actual channel counts
    long total_input_channels = 0;
    for (int i = 0; i < x->input_count; i++) {
        long inlet_channels = (long)object_method(dsp64, gensym("getnuminputchannels"), x, i);
        total_input_channels += inlet_channels;
        post("Inlet %d has %ld channels", i, inlet_channels);
    }

    post("zerr_combinator_dsp64: total_input_channels=%ld, samplerate=%.0f",
        total_input_channels, samplerate);

    long inletchannelcount = (long)object_method(dsp64, gensym("getnuminputchannels"), x, 0);
    for (int i = 1; i < x->input_count; ++i) {
        if (inletchannelcount != (long)object_method(dsp64, gensym("getnuminputchannels"), x, i)) {
            x->channel_muted = TRUE;
            post("zerr_combinator_dsp64: muted");
            return;
        }
    }
    x->channel_muted = FALSE;
    post("zerr_combinator_dsp64: enabled");
    dsp_add64(dsp64, (t_object*)x, (t_perfroutine64)zerr_combinator_perform64, 0, NULL);
}

//--------------------------------------------------------------------------

void zerr_combinator_perform64(t_zerr_combinator* x, t_object* dsp64, double** ins, long numins, double** outs, long numouts, long sampleframes, long flags, void* userparam)
{
    if (x->channel_muted) {
        // Clear all outputs if we have an invalid state
        for (long i = 0; i < numouts; i++) {
            if (outs[i]) {
                memset(outs[i], 0, sampleframes * sizeof(double));
            }
        }
        return;
    }

    long num_groups = x->input_count;

    for (long channel = 0; channel < numouts; channel++) {
        // Process each sample for this channel
        for (long sample = 0; sample < sampleframes; sample++) {
            double max_val = 0.0;

            // Check all groups for this channel
            for (long group = 0; group < num_groups; group++) {
                long input_idx = group * numouts + channel;

                double val = ins[input_idx][sample];
                max_val = (val > max_val) ? val : max_val;
            }

            // Handle case where all inputs were NULL
            outs[channel][sample] = max_val;
        }
    }
}

//--------------------------------------------------------------------------

void zerr_combinator_bang(t_zerr_combinator* x)
{
    post("mc.zerrcombinator~: current channel count = %ld", x->channel_count);
}
