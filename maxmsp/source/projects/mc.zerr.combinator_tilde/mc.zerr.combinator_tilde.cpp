/**
 * @file    mc.zerr.combinator_tilde.cpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   mc.zerr.combinator~ Max/MSP External - Multichannel signal combinator
 * @date    2025-05-03
 *
 * @copyright  Copyright (c) 2023-2025 Zeyu Yang
 * @license    MIT license
 */

#include "commonsyms.h"
#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

#include "./zerr_combinator.hpp"

//------------------------------------------------------------------------------
// Type Definitions
//------------------------------------------------------------------------------

/**
 * @struct t_zerr_combinator
 * @brief Main data structure for the mc.zerr.combinator~ object
 */
typedef struct _zerr_combinator {
    t_pxobject x_obj; ///< DSP object header (must be first)
    long channel_count; ///< Channel count of multichannel signal
    long input_count; ///< Number of input channel groups
    bool channel_muted; ///< Mute flag for unequal channel counts
    ZerrCombinator* zc; ///< Pointer to the zerr_combinator implementation
} t_zerr_combinator;

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

void* zerr_combinator_new(t_symbol* s, long argc, t_atom* argv);

void zerr_combinator_free(t_zerr_combinator* x);

void zerr_combinator_assist(t_zerr_combinator* x, void* b, long m, long a, char* s);

void zerr_combinator_dsp64(t_zerr_combinator* x, t_object* dsp64, short* count,
    double samplerate, long maxvectorsize, long flags);

void zerr_combinator_perform64(t_zerr_combinator* x, t_object* dsp64,
    double** ins, long numins, double** outs,
    long numouts, long sampleframes, long flags,
    void* userparam);

void zerr_combinator_bang(t_zerr_combinator* x);

long zerr_combinator_multichanneloutputs(t_zerr_combinator* x, long outletindex);

long zerr_combinator_inputchanged(t_zerr_combinator* x, long index, long count);

// Class pointer
static t_class* zerr_combinator_class = NULL;

//------------------------------------------------------------------------------
// Main Entry Point
//------------------------------------------------------------------------------

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

    // Register methods
    class_addmethod(c, (method)zerr_combinator_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)zerr_combinator_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)zerr_combinator_multichanneloutputs, "multichanneloutputs", A_CANT, 0);
    class_addmethod(c, (method)zerr_combinator_inputchanged, "inputchanged", A_CANT, 0);
    class_addmethod(c, (method)zerr_combinator_bang, "bang", 0);

    // Attributes
    CLASS_ATTR_LONG(c, "chans", 0, t_zerr_combinator, channel_count);
    CLASS_ATTR_LABEL(c, "chans", 0, "Output Channels");
    CLASS_ATTR_FILTER_CLIP(c, "chans", 1, MC_MAX_CHANS);
    CLASS_ATTR_BASIC(c, "chans", 0);

    // Initialize DSP and register the class
    class_dspinit(c);
    class_register(CLASS_BOX, c);

    zerr_combinator_class = c;
}

//------------------------------------------------------------------------------
// Object Creation/Destruction
//------------------------------------------------------------------------------

void* zerr_combinator_new(t_symbol* s, long argc, t_atom* argv)
{
    t_zerr_combinator* x = (t_zerr_combinator*)object_alloc(zerr_combinator_class);

    if (x) {
        // Initialize default values
        x->zc = NULL;
        x->channel_count = 1; // Default: 1 channel output
        x->input_count = 2; // Default: combine two sets of envelopes
        x->channel_muted = FALSE;

        // Validate arguments
        if (argc < 2) {
            object_error((t_object*)x, "requires 2 arguments");
            return NULL;
        }

        // Process argument 1: number of inputs
        if (atom_gettype(argv) == A_LONG) {
            x->input_count = atom_getlong(argv);
            if (x->input_count < 1) {
                object_error((t_object*)x, "input_count must be at least 1");
                return NULL;
            }
        } else {
            object_error((t_object*)x, "first argument must be an integer");
            return NULL;
        }

        // Process argument 2: combine mode
        if (atom_gettype(argv + 1) != A_SYM) {
            object_error((t_object*)x, "second argument must be a symbol");
            return NULL;
        }
        t_symbol* arg2 = atom_getsym(argv + 1);
        const char* mode = arg2->s_name;

        // TODO: Initialize the ZerrCombinator based on the mode

        // Process attributes
        attr_args_process(x, argc, argv);

        // Set up DSP with the number of inlets
        dsp_setup((t_pxobject*)x, x->input_count);
        x->x_obj.z_misc = Z_MC_INLETS;

        // Create multichannel outlet
        outlet_new((t_object*)x, "multichannelsignal");

        // object_post((t_object*)x, "created with %ld inputs", x->input_count);
    }

    return x;
}

void zerr_combinator_free(t_zerr_combinator* x)
{
    dsp_free((t_pxobject*)x);

    // Clean up the ZerrCombinator object if it exists
    if (x->zc) {
        delete x->zc;
        x->zc = NULL;
    }
}

//------------------------------------------------------------------------------
// UI and Info Methods
//------------------------------------------------------------------------------

void zerr_combinator_assist(t_zerr_combinator* x, void* b, long m, long a, char* s)
{
    if (m == ASSIST_INLET) {
        snprintf(s, 128, "Signal Input %ld: Envelope Group", a + 1);
    } else if (m == ASSIST_OUTLET) {
        strcpy(s, "(multichannel signal) Combined Envelopes");
    }
}

void zerr_combinator_bang(t_zerr_combinator* x)
{
    object_post((t_object*)x, "combination mode: %s \n channel count = %ld", "everything", x->channel_count);
}

//------------------------------------------------------------------------------
// Multichannel Methods
//------------------------------------------------------------------------------

long zerr_combinator_multichanneloutputs(t_zerr_combinator* x, long outletindex)
{
    return x->channel_count;
}

long zerr_combinator_inputchanged(t_zerr_combinator* x, long index, long count)
{
    if (index == 0) {
        x->channel_count = count;
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
// DSP Methods
//------------------------------------------------------------------------------

void zerr_combinator_dsp64(t_zerr_combinator* x, t_object* dsp64, short* count,
    double samplerate, long maxvectorsize, long flags)
{

    // Get actual channel counts for diagnostics
    long total_input_channels = 0;
    for (int i = 0; i < x->input_count; i++) {
        long inlet_channels = (long)object_method(dsp64, gensym("getnuminputchannels"), x, i);
        total_input_channels += inlet_channels;
    }

    // Verify all inlets have the same channel count
    long inletchannelcount = (long)object_method(dsp64, gensym("getnuminputchannels"), x, 0);
    for (int i = 1; i < x->input_count; ++i) {
        if (inletchannelcount != (long)object_method(dsp64, gensym("getnuminputchannels"), x, i)) {
            x->channel_muted = TRUE;
            return;
        }
    }

    // All channel counts are equal, enable audio processing
    x->channel_muted = FALSE;
    dsp_add64(dsp64, (t_object*)x, (t_perfroutine64)zerr_combinator_perform64, 0, NULL);
}

void zerr_combinator_perform64(t_zerr_combinator* x, t_object* dsp64,
    double** ins, long numins, double** outs,
    long numouts, long sampleframes, long flags,
    void* userparam)
{
    // Zero out the output if the input signals don't have equal channel counts
    if (x->channel_muted) {
        for (long i = 0; i < numouts; i++) {
            if (outs[i]) {
                memset(outs[i], 0, sampleframes * sizeof(double));
            }
        }
        return;
    }

    long num_groups = x->input_count;

    // Process each output channel
    for (long channel = 0; channel < numouts; channel++) {
        // Process each sample for this channel
        for (long sample = 0; sample < sampleframes; sample++) {
            double max_val = 0.0;

            // Find maximum value across all input groups for this channel
            for (long group = 0; group < num_groups; group++) {
                long input_idx = group * numouts + channel;
                if (input_idx < numins && ins[input_idx]) {
                    double val = ins[input_idx][sample];
                    max_val = (val > max_val) ? val : max_val;
                }
            }

            // Output the maximum value
            outs[channel][sample] = max_val;
        }
    }
}