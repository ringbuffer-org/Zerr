/**
 * @file    zerr.features_tilde.cpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   zerr.features~ Max/MSP External - one signal outlet per audio feature
 * @date    2025-05-01
 *
 * @copyright  Copyright (c) 2023-2026 Zeyu Yang
 * @license    MIT license
 */

#include "commonsyms.h"
#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

#include "./zerr_features.hpp"

#include <string>

//------------------------------------------------------------------------------
// Type Definitions
//------------------------------------------------------------------------------

/**
 * @struct t_zerr_features
 * @brief Main data structure for the zerr.features~ object
 */
typedef struct _zerr_features {
    t_pxobject x_obj;   ///< DSP object header (must be first)
    long channel_count; ///< Number of signal outlets, one per feature
    bool dsp_ready;     ///< dsp_setup() has run, so free must call dsp_free()
    ZerrFeatures* zf;   ///< Pointer to the zerr_features implementation
} t_zerr_features;

#ifndef ZERR_MAX_VERSION
#define ZERR_MAX_VERSION "dev"
#endif

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

void* zerr_features_new(t_symbol* s, long argc, t_atom* argv);
void zerr_features_free(t_zerr_features* x);
void zerr_features_assist(t_zerr_features* x, void* b, long m, long a, char* s);
void zerr_features_dsp64(t_zerr_features* x, t_object* dsp64, short* count, double samplerate,
                         long maxvectorsize, long flags);
void zerr_features_perform64(t_zerr_features* x, t_object* dsp64, double** ins, long numins,
                             double** outs, long numouts, long sampleframes, long flags,
                             void* userparam);
void zerr_features_bang(t_zerr_features* x);

// Class pointer
static t_class* zerr_features_class = NULL;

//------------------------------------------------------------------------------
// Main Entry Point
//------------------------------------------------------------------------------

C74_EXPORT void ext_main(void* r)
{
    t_class* c;

    c = class_new("zerr.features~", (method)zerr_features_new, (method)zerr_features_free,
                  sizeof(t_zerr_features), 0L, A_GIMME, 0);

    class_addmethod(c, (method)zerr_features_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)zerr_features_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)zerr_features_bang, "bang", 0);

    // Initialize DSP and register the class
    class_dspinit(c);
    class_register(CLASS_BOX, c);

    zerr_features_class = c;
}

//------------------------------------------------------------------------------
// Object Creation/Destruction
//------------------------------------------------------------------------------

// Space-separated list of the feature names the core registers, for error messages.
static std::string zerr_features_valid_names()
{
    std::string out;
    for (const auto& name : zerr::FeatureBank::availableFeatures()) {
        if (!out.empty()) {
            out += ' ';
        }
        out += name;
    }
    return out;
}

// Every failure path in new() ends here. object_free() runs zerr_features_free(), which
// tolerates every partial state (no wrapper yet, dsp_setup() not yet run), so neither the
// Max object nor the wrapper leaks.
static void* zerr_features_fail(t_zerr_features* x)
{
    object_free((t_object*)x);
    return NULL;
}

void* zerr_features_new(t_symbol* s, long argc, t_atom* argv)
{
    t_zerr_features* x = (t_zerr_features*)object_alloc(zerr_features_class);
    if (!x)
        return NULL;

    x->zf            = NULL;
    x->dsp_ready     = false;
    x->channel_count = 1;

    // Positional arguments are feature names; they end at the first @attribute.
    const long offset       = attr_args_offset(argc, argv);
    const std::string valid = zerr_features_valid_names();

    zerr::FeatureNames featureNames;
    if (offset < 1) {
        // Plain [zerr.features~] works: one loudness outlet.
        featureNames.push_back("rms");
        object_post((t_object*)x, "no feature given, using rms (available: %s)", valid.c_str());
    }
    for (long i = 0; i < offset; i++) {
        if (atom_gettype(argv + i) != A_SYM) {
            object_error((t_object*)x, "argument %ld must be a feature name (available: %s)", i + 1,
                         valid.c_str());
            return zerr_features_fail(x);
        }
        featureNames.push_back(atom_getsym(argv + i)->s_name);
    }
    x->channel_count = (long)featureNames.size();

    // Build the core once now so a misspelled name fails creation with a message, using
    // Max's global rate and vector size; dsp64 rebuilds for the real signal chain.
    x->zf = new ZerrFeatures(featureNames);
    zerr::SystemConfigs cfgs;
    cfgs.sample_rate = (size_t)sys_getsr();
    cfgs.block_size  = (size_t)sys_getblksize();
    std::string error;
    if (!x->zf->prepare(cfgs, &error)) {
        object_error((t_object*)x, "%s (available: %s)", error.c_str(), valid.c_str());
        return zerr_features_fail(x);
    }

    attr_args_process(x, argc, argv);

    for (long i = 0; i < x->channel_count; ++i) {
        outlet_new((t_object*)x, "signal");
    }

    dsp_setup((t_pxobject*)x, x->zf->getInputCount());
    x->dsp_ready = true;

    return x;
}

void zerr_features_free(t_zerr_features* x)
{
    // dsp_free() must pair with dsp_setup(); a creation that failed earlier never ran it.
    if (x->dsp_ready) {
        dsp_free((t_pxobject*)x);
    }
    delete x->zf;
    x->zf = NULL;
}

//------------------------------------------------------------------------------
// UI and Info Methods
//------------------------------------------------------------------------------

void zerr_features_assist(t_zerr_features* x, void* b, long m, long a, char* s)
{
    if (m == ASSIST_INLET) {
        strcpy(s, "(signal) mono input to analyse");
    }
    else if (m == ASSIST_OUTLET) {
        const auto& names = x->zf->getFeatureNames();
        if (a >= 0 && a < (long)names.size()) {
            snprintf(s, 256, "(signal) %s, feature %ld of %ld", names[a].c_str(), a + 1,
                     (long)names.size());
        }
        else {
            strcpy(s, "(signal) audio feature");
        }
    }
}

//------------------------------------------------------------------------------
// DSP Methods
//------------------------------------------------------------------------------

void zerr_features_dsp64(t_zerr_features* x, t_object* dsp64, short* count, double samplerate,
                         long maxvectorsize, long flags)
{
    // The chain's own rate and vector size, which differ from the global ones inside a
    // resampled poly~ or after a change in Audio Status. prepare() is idempotent when
    // nothing changed; on failure perform() keeps outputting silence.
    zerr::SystemConfigs cfgs;
    cfgs.sample_rate = (size_t)samplerate;
    cfgs.block_size  = (size_t)maxvectorsize;
    std::string error;
    if (!x->zf->prepare(cfgs, &error)) {
        object_error((t_object*)x, "cannot prepare for %ld samples at %.0f Hz: %s; output muted",
                     maxvectorsize, samplerate, error.c_str());
    }

    dsp_add64(dsp64, (t_object*)x, (t_perfroutine64)zerr_features_perform64, 0, NULL);
}

void zerr_features_perform64(t_zerr_features* x, t_object* dsp64, double** ins, long numins,
                             double** outs, long numouts, long sampleframes, long flags,
                             void* userparam)
{
    x->zf->perform(ins, numins, outs, numouts, sampleframes); // noexcept
}

//------------------------------------------------------------------------------

void zerr_features_bang(t_zerr_features* x)
{
    std::string names;
    for (const auto& name : x->zf->getFeatureNames()) {
        if (!names.empty()) {
            names += ' ';
        }
        names += name;
    }
    const auto& cfgs = x->zf->getSystemConfigs();
    object_post((t_object*)x, "zerr.features~ %s: %ld feature(s) [%s], block %ld, sr %ld",
                ZERR_MAX_VERSION, x->channel_count, names.c_str(), (long)cfgs.block_size,
                (long)cfgs.sample_rate);
}
