/**
 * @file    mc.zerr.features_tilde.cpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   mc.zerr.features~ Max/MSP External using Max API for better multi-channel support
 * @date    2025-05-01
 *
 * @copyright  Copyright (c) 2023-2025 Zeyu Yang
 * @license    MIT license
 */

#include "commonsyms.h"
#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

#include "./zerr_features.hpp"

//------------------------------------------------------------------------------
// Type Definitions
//------------------------------------------------------------------------------

/**
 * @struct t_zerr_features
 * @brief Main data structure for the mc.zerr.features~ object
 */
typedef struct _zerr_features {
    t_pxobject x_obj; ///< DSP object header (must be first)
    long channel_count; ///< Channel count of multichannel signal
    ZerrFeatures* zf; ///< Pointer to the zerr_features implementation
} t_zerr_features;

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

void* zerr_features_new(t_symbol* s, long argc, t_atom* argv);
void zerr_features_free(t_zerr_features* x);
void zerr_features_assist(t_zerr_features* x, void* b, long m, long a, char* s);
void zerr_features_dsp64(t_zerr_features* x, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags);
void zerr_features_perform64(t_zerr_features* x, t_object* dsp64, double** ins, long numins, double** outs, long numouts, long sampleframes, long flags, void* userparam);
long zerr_features_multichanneloutputs(t_zerr_features* x, long outletindex);
void zerr_features_bang(t_zerr_features* x);

// Class pointer
static t_class* zerr_features_class = NULL;

//------------------------------------------------------------------------------
// Main Entry Point
//------------------------------------------------------------------------------

C74_EXPORT void ext_main(void* r)
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

    // CLASS_ATTR_LONG(c, "chans", 0, t_zerr_features, channel_count);
    // CLASS_ATTR_LABEL(c, "chans", 0, "Output Channels");
    // CLASS_ATTR_FILTER_CLIP(c, "chans", 1, MC_MAX_CHANS);
    // CLASS_ATTR_BASIC(c, "chans", 0);

    // Initialize DSP and register the class
    class_dspinit(c);
    class_register(CLASS_BOX, c);

    zerr_features_class = c;
}

//------------------------------------------------------------------------------
// Object Creation/Destruction
//------------------------------------------------------------------------------

void* zerr_features_new(t_symbol* s, long argc, t_atom* argv)
{
    // Initialize t_zerr_features structure ------------------------------------
    t_zerr_features* x = (t_zerr_features*)object_alloc(zerr_features_class);
    if (!x)
        return NULL;

    // Initialize default values -----------------------------------------------
    x->zf = NULL;
    x->channel_count = 1; // Default 1 channel output for the multichannel outlet

    // Parsing arguments -------------------------------------------------------
    long offset = attr_args_offset(argc, argv);
    if (offset < 1) {
        object_error((t_object*)x, "at least one feature should be assgined!");
        return NULL;
    }

    x->channel_count = offset; // the number of argument defines the channel

    // Ensure channel count is within bounds
    // Which is basically not possible (1024 channel)
    x->channel_count = CLAMP(x->channel_count, 1, MC_MAX_CHANS);

    zerr::FeatureNames featureNames;
    featureNames.reserve(argc);

    // Copy arguments to feature names vector
    for (int i = 0; i < argc; i++) {
        featureNames.push_back(atom_getsym(argv + i)->s_name);
    }

    // create & initialize ZerrFeatures instance ------------------------------
    x->zf = new ZerrFeatures(sys_getsr(), sys_getblksize(), featureNames);
    if (!x->zf)
        return NULL;
    if (!x->zf->initialize()) {
        delete x->zf;
        return NULL;
    }

    // Further instance setups -------------------------------------------------
    attr_args_process(x, argc, argv);

    dsp_setup((t_pxobject*)x, 1);

    // Mark as multichannel inlet enabled
    // x->x_obj.z_misc = Z_MC_INLETS;

    outlet_new((t_object*)x, "multichannelsignal");

    return x;
}

void zerr_features_free(t_zerr_features* x)
{
    dsp_free((t_pxobject*)x);
    if (x->zf) {
        delete x->zf;
        x->zf = NULL;
    }
}

//------------------------------------------------------------------------------
// UI and Info Methods
//------------------------------------------------------------------------------

void zerr_features_assist(t_zerr_features* x, void* b, long m, long a, char* s)
{
    if (m == ASSIST_INLET) {
        strcpy(s, "(signal) Input source signal");
    } else if (m == ASSIST_OUTLET) {
        strcpy(s, "(multichannel signal) Output extracted audio features");
    }
}

//------------------------------------------------------------------------------
// Multichannel Methods
//------------------------------------------------------------------------------

long zerr_features_multichanneloutputs(t_zerr_features* x, long outletindex)
{
    return x->channel_count;
}

//------------------------------------------------------------------------------
// DSP Methods
//------------------------------------------------------------------------------

void zerr_features_dsp64(t_zerr_features* x, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags)
{
    dsp_add64(dsp64, (t_object*)x, (t_perfroutine64)zerr_features_perform64, 0, NULL);
}

void zerr_features_perform64(t_zerr_features* x, t_object* dsp64, double** ins, long numins, double** outs, long numouts, long sampleframes, long flags, void* userparam)
{
    x->zf->perform(ins, numins, outs, numouts, sampleframes);
}

//--------------------------------------------------------------------------

void zerr_features_bang(t_zerr_features* x)
{
    // change to output current activate feature info
    object_post((t_object*)x, "current channel count = %ld", x->channel_count);
}
