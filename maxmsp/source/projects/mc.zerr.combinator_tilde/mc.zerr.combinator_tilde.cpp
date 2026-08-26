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

#include <exception>

/// Combination mode used when the object is created without a mode argument.
/// "max" is what this object has always computed, so existing patches are unaffected.
#define ZERR_COMBINATOR_DEFAULT_MODE "max"

//------------------------------------------------------------------------------
// Type Definitions
//------------------------------------------------------------------------------

/**
 * @struct t_zerr_combinator
 * @brief Main data structure for the mc.zerr.combinator~ object
 */
typedef struct _zerr_combinator {
    t_pxobject x_obj;   ///< DSP object header (must be first)
    long channel_count; ///< Envelopes per set, derived from the inlets' channel count
    long input_count;   ///< Number of envelope sets, one multichannel inlet each
    t_symbol* mode_sym; ///< Combination mode in use, for bang/attribute reporting
    bool channel_muted; ///< Mute flag for unequal channel counts
    ZerrCombinator* zc; ///< Pointer to the zerr_combinator implementation
} t_zerr_combinator;

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

void* zerr_combinator_new(t_symbol* s, long argc, t_atom* argv);

void zerr_combinator_free(t_zerr_combinator* x);

void zerr_combinator_assist(t_zerr_combinator* x, void* b, long m, long a, char* s);

void zerr_combinator_dsp64(t_zerr_combinator* x, t_object* dsp64, short* count, double samplerate,
                           long maxvectorsize, long flags);

void zerr_combinator_perform64(t_zerr_combinator* x, t_object* dsp64, double** ins, long numins,
                               double** outs, long numouts, long sampleframes, long flags,
                               void* userparam);

void zerr_combinator_bang(t_zerr_combinator* x);

void zerr_combinator_mode(t_zerr_combinator* x, t_symbol* msg, long argc, t_atom* argv);

t_max_err zerr_combinator_mode_set(t_zerr_combinator* x, t_object* attr, long argc, t_atom* argv);

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

    c = class_new("mc.zerr.combinator~", (method)zerr_combinator_new, (method)zerr_combinator_free,
                  sizeof(t_zerr_combinator), 0L, A_GIMME, 0);

    // Register methods
    class_addmethod(c, (method)zerr_combinator_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)zerr_combinator_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)zerr_combinator_multichanneloutputs, "multichanneloutputs", A_CANT,
                    0);
    class_addmethod(c, (method)zerr_combinator_inputchanged, "inputchanged", A_CANT, 0);
    class_addmethod(c, (method)zerr_combinator_bang, "bang", 0);
    class_addmethod(c, (method)zerr_combinator_mode, "mode", A_GIMME, 0);

    // Attributes. "chans" is derived from the inlets' channel count in dsp64, so it is
    // readable but not settable -- writing it would only desync it from the real signal.
    CLASS_ATTR_LONG(c, "chans", ATTR_SET_OPAQUE_USER, t_zerr_combinator, channel_count);
    CLASS_ATTR_LABEL(c, "chans", 0, "Output Channels (read-only)");
    CLASS_ATTR_BASIC(c, "chans", 0);

    // The same switch as the "mode" message, so it also works from the inspector and as
    // an @mode attribute at object creation.
    CLASS_ATTR_SYM(c, "mode", 0, t_zerr_combinator, mode_sym);
    CLASS_ATTR_ACCESSORS(c, "mode", NULL, zerr_combinator_mode_set);
    CLASS_ATTR_ENUM(c, "mode", 0, "\"add\" \"root\" \"max\"");
    CLASS_ATTR_LABEL(c, "mode", 0, "Combination Mode");
    CLASS_ATTR_BASIC(c, "mode", 0);

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
        x->zc            = NULL;
        x->channel_count = 1; // Until dsp64 reports the inlets' channel count
        x->input_count   = 2; // Default: combine two sets of envelopes
        x->mode_sym      = gensym(ZERR_COMBINATOR_DEFAULT_MODE);
        x->channel_muted = FALSE;

        // Positional arguments end at the first @attribute
        long offset = attr_args_offset(argc, argv);

        // Validate arguments
        if (offset < 1) {
            object_error((t_object*)x,
                         "requires at least 1 argument: <number of envelope sets> [mode]");
            return NULL;
        }

        // Process argument 1: number of envelope sets
        if (atom_gettype(argv) != A_LONG) {
            object_error((t_object*)x,
                         "first argument must be an integer (number of envelope sets)");
            return NULL;
        }
        x->input_count = atom_getlong(argv);
        if (x->input_count < 1) {
            object_error((t_object*)x, "number of envelope sets must be at least 1");
            return NULL;
        }

        // Process argument 2: combine mode. Optional, defaults to "max".
        // A bad mode name is recoverable, so report it and fall back rather than
        // leaving the user with a dead object box.
        const char* mode = ZERR_COMBINATOR_DEFAULT_MODE;
        if (offset > 1) {
            if (atom_gettype(argv + 1) != A_SYM) {
                object_error((t_object*)x, "second argument must be a symbol (combination mode)");
                return NULL;
            }
            const char* requested = atom_getsym(argv + 1)->s_name;
            zerr::CombMode parsed;
            if (zerr::tryParseCombMode(requested, parsed)) {
                mode = requested;
            }
            else {
                object_error((t_object*)x,
                             "unknown combination mode '%s'; expected %s. Using '%s'.", requested,
                             zerr::combModeNames(), mode);
            }
        }
        x->mode_sym = gensym(mode);

        // Build the wrapper. The core module itself is built in dsp64, once the
        // channel count, sample rate and block size are known.
        try {
            x->zc = new ZerrCombinator((int)x->input_count, mode);
        }
        catch (const std::exception& e) {
            object_error((t_object*)x, "failed to create combinator: %s", e.what());
            return NULL;
        }

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
        // The number of envelopes per set is the inlet's own channel count, and every
        // inlet must agree -- it is not an object argument.
        snprintf(
            s, 128,
            "(multichannel signal) Envelope Set %ld; all sets must have the same channel count",
            a + 1);
    }
    else if (m == ASSIST_OUTLET) {
        strcpy(s, "(multichannel signal) Combined Envelopes");
    }
}

void zerr_combinator_bang(t_zerr_combinator* x)
{
    object_post((t_object*)x, "combination mode: %s, envelope sets: %ld, envelopes per set: %ld",
                x->mode_sym ? x->mode_sym->s_name : "unknown", x->input_count, x->channel_count);
}

/**
 * Applies a combination mode by name. Shared by the "mode" message and the "mode"
 * attribute so the two can never diverge. Control thread only -- the core publishes the
 * change through a lock-free atomic, so no deferral is needed and DSP can keep running.
 */
static bool zerr_combinator_apply_mode(t_zerr_combinator* x, t_symbol* mode)
{
    if (!mode || !x->zc || !x->zc->setMode(mode->s_name)) {
        object_error((t_object*)x, "mode: unknown combination mode '%s'; expected %s",
                     mode ? mode->s_name : "", zerr::combModeNames());
        return false;
    }
    x->mode_sym = mode;
    return true;
}

void zerr_combinator_mode(t_zerr_combinator* x, t_symbol* msg, long argc, t_atom* argv)
{
    if (argc < 1 || atom_gettype(argv) != A_SYM) {
        object_error((t_object*)x, "mode: expects one symbol (%s)", zerr::combModeNames());
        return;
    }
    zerr_combinator_apply_mode(x, atom_getsym(argv));
}

t_max_err zerr_combinator_mode_set(t_zerr_combinator* x, t_object* attr, long argc, t_atom* argv)
{
    if (argc < 1 || !argv || atom_gettype(argv) != A_SYM) {
        object_error((t_object*)x, "mode: expects one symbol (%s)", zerr::combModeNames());
        return MAX_ERR_GENERIC;
    }
    return zerr_combinator_apply_mode(x, atom_getsym(argv)) ? MAX_ERR_NONE : MAX_ERR_GENERIC;
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

void zerr_combinator_dsp64(t_zerr_combinator* x, t_object* dsp64, short* count, double samplerate,
                           long maxvectorsize, long flags)
{

    // Verify all inlets have the same channel count. The perform routine is added even
    // when they don't, so that the muted path actually silences the outlet instead of
    // leaving whatever the previous DSP chain wrote there.
    long inletchannelcount = (long)object_method(dsp64, gensym("getnuminputchannels"), x, 0);
    for (int i = 1; i < x->input_count; ++i) {
        if (inletchannelcount != (long)object_method(dsp64, gensym("getnuminputchannels"), x, i)) {
            object_error((t_object*)x,
                         "all envelope sets must have the same channel count; output muted");
            x->channel_muted = TRUE;
            dsp_add64(dsp64, (t_object*)x, (t_perfroutine64)zerr_combinator_perform64, 0, NULL);
            return;
        }
    }

    x->channel_count = inletchannelcount;

    // Build the core module for this signal shape. Take the sample rate and block size
    // from the dsp64 arguments rather than sys_getsr()/sys_getblksize(), which differ
    // inside poly~.
    zerr::SystemConfigs cfgs;
    cfgs.sample_rate = (size_t)samplerate;
    cfgs.block_size  = (size_t)maxvectorsize;

    if (!x->zc || !x->zc->prepare(cfgs, (int)x->channel_count)) {
        object_error((t_object*)x, "failed to configure combinator for %ld channels; output muted",
                     inletchannelcount);
        x->channel_muted = TRUE;
    }
    else {
        x->channel_muted = FALSE;
    }

    dsp_add64(dsp64, (t_object*)x, (t_perfroutine64)zerr_combinator_perform64, 0, NULL);
}

void zerr_combinator_perform64(t_zerr_combinator* x, t_object* dsp64, double** ins, long numins,
                               double** outs, long numouts, long sampleframes, long flags,
                               void* userparam)
{
    // Zero out the output if the input signals don't have equal channel counts
    if (x->channel_muted || !x->zc) {
        for (long i = 0; i < numouts; i++) {
            if (outs[i]) {
                memset(outs[i], 0, sampleframes * sizeof(double));
            }
        }
        return;
    }

    x->zc->perform(ins, numins, outs, numouts, sampleframes);
}