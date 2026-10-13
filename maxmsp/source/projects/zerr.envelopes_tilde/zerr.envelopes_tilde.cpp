/**
 * @file    zerr.envelopes_tilde.cpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   zerr.envelopes~ Max/MSP External - Multichannel envelope generator
 * @date    2025-05-03
 *
 * @copyright  Copyright (c) 2023-2026
 * @license    MIT license
 */

#include "commonsyms.h"
#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

#include "./zerr_envelopes.hpp"
#include "zerr_default_layout.hpp"
#include "zerr_max_log.hpp"

#include <string>

//------------------------------------------------------------------------------
// Type Definitions
//------------------------------------------------------------------------------

/**
 * @struct t_zerr_envelopes
 * @brief Main data structure for the zerr.envelopes~ object
 */
typedef struct _zerr_envelopes {
    t_pxobject x_obj;   ///< DSP object header (must be first)
    long channel_count; ///< Channel count of the multichannel outlet (one per speaker)
    bool dsp_ready;     ///< dsp_setup() has run, so free must call dsp_free()
    ZerrEnvelopes* ze;  ///< Pointer to the zerr_envelopes implementation
} t_zerr_envelopes;

#ifndef ZERR_MAX_VERSION
#define ZERR_MAX_VERSION "dev"
#endif

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

void* zerr_envelopes_new(t_symbol* s, long argc, t_atom* argv);
void zerr_envelopes_free(t_zerr_envelopes* x);
void zerr_envelopes_assist(t_zerr_envelopes* x, void* b, long m, long a, char* s);
void zerr_envelopes_dsp64(t_zerr_envelopes* x, t_object* dsp64, short* count, double samplerate,
                          long maxvectorsize, long flags);
void zerr_envelopes_perform64(t_zerr_envelopes* x, t_object* dsp64, double** ins, long numins,
                              double** outs, long numouts, long sampleframes, long flags,
                              void* userparam);
long zerr_envelopes_multichanneloutputs(t_zerr_envelopes* x, long outletindex);
//------------------------------------------------------------------------------
void zerr_envelopes_bang(t_zerr_envelopes* x);
void zerr_envelopes_active(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv);
void zerr_envelopes_curr(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv);
void zerr_envelopes_topo(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv);
void zerr_envelopes_traj(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv);
void zerr_envelopes_interval(t_zerr_envelopes* x, t_symbol* msg, long argc, t_atom* argv);
void zerr_envelopes_print(t_zerr_envelopes* x);

// Class pointer
static t_class* zerr_envelopes_class = NULL;

//------------------------------------------------------------------------------
// Main Entry Point
//------------------------------------------------------------------------------

C74_EXPORT void ext_main(void* r)
{
    t_class* c;

    c = class_new("zerr.envelopes~", (method)zerr_envelopes_new, (method)zerr_envelopes_free,
                  sizeof(t_zerr_envelopes), 0L, A_GIMME, 0);

    // Register methods
    class_addmethod(c, (method)zerr_envelopes_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)zerr_envelopes_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)zerr_envelopes_multichanneloutputs, "multichanneloutputs", A_CANT,
                    0);
    class_addmethod(c, (method)zerr_envelopes_bang, "bang", 0);
    class_addmethod(c, (method)zerr_envelopes_active, "active", A_GIMME, 0);
    class_addmethod(c, (method)zerr_envelopes_curr, "curr", A_GIMME, 0);
    class_addmethod(c, (method)zerr_envelopes_topo, "topo", A_GIMME, 0);
    class_addmethod(c, (method)zerr_envelopes_traj, "traj", A_GIMME, 0);
    class_addmethod(c, (method)zerr_envelopes_interval, "interval", A_GIMME, 0);
    class_addmethod(c, (method)zerr_envelopes_print, "print", 0);

    // Initialize DSP and register the class
    class_dspinit(c);
    class_register(CLASS_BOX, c);

    zerr_envelopes_class = c;
}

//------------------------------------------------------------------------------
// Object Creation/Destruction
//------------------------------------------------------------------------------

// Every failure path in new() ends here. object_free() runs zerr_envelopes_free(), which
// tolerates every partial state (no wrapper yet, dsp_setup() not yet run), so neither the
// Max object nor the wrapper leaks.
static void* zerr_envelopes_fail(t_zerr_envelopes* x)
{
    object_free((t_object*)x);
    return NULL;
}

// Resolves a layout name through Max's search path: as given, then with .yaml, then with
// .yml. Returns the first hit as a Unix path, or an empty string.
static std::string zerr_envelopes_find_layout(t_zerr_envelopes* x, const char* name)
{
    t_fourcc file_types[] = {'TEXT', 'YAML'};
    const size_t len      = strlen(name);
    const bool has_ext    = (len > 5 && strcmp(name + len - 5, ".yaml") == 0) ||
                            (len > 4 && strcmp(name + len - 4, ".yml") == 0);

    const char* suffixes[] = {"", ".yaml", ".yml"};
    for (const char* suffix : suffixes) {
        if (suffix[0] == '\0' && !has_ext) {
            continue; // a bare name is only tried with an extension
        }
        char candidate[MAX_PATH_CHARS];
        snprintf(candidate, MAX_PATH_CHARS, "%s%s", name, suffix);

        t_symbol* absolute = NULL;
        if (path_absolutepath(&absolute, gensym(candidate), file_types, 2) == MAX_ERR_NONE &&
            absolute) {
            // Max paths look like "Macintosh HD:/Users/..."; the core wants "/Users/...".
            const char* full  = absolute->s_name;
            const char* colon = strchr(full, ':');
            std::string unix_path(colon ? colon + 1 : full);
            object_post((t_object*)x, "speaker layout: %s", unix_path.c_str());
            return unix_path;
        }
    }
    return std::string();
}

void* zerr_envelopes_new(t_symbol* s, long argc, t_atom* argv)
{
    t_zerr_envelopes* x = (t_zerr_envelopes*)object_alloc(zerr_envelopes_class);
    if (!x)
        return NULL;

    x->ze            = NULL;
    x->dsp_ready     = false;
    x->channel_count = 1;

    // Positional arguments: [mode] [layout]; they end at the first @attribute.
    const long offset = attr_args_offset(argc, argv);
    if (offset > 2) {
        object_error((t_object*)x,
                     "expects at most 2 arguments: [trigger|trajectory] [speaker layout]");
        return zerr_envelopes_fail(x);
    }

    // Argument 1: mode. Defaults to trajectory.
    std::string mode = "trajectory";
    if (offset >= 1) {
        if (atom_gettype(argv) != A_SYM) {
            object_error((t_object*)x, "first argument must be trigger or trajectory");
            return zerr_envelopes_fail(x);
        }
        mode = atom_getsym(argv)->s_name;
        try {
            zerr::parseGenMode(mode);
        }
        catch (const std::exception&) {
            object_error((t_object*)x, "unknown mode '%s'; expected trigger or trajectory",
                         mode.c_str());
            return zerr_envelopes_fail(x);
        }
    }

    // Argument 2: speaker layout. Defaults to a built-in 8-speaker ring, so a plain
    // [zerr.envelopes~] works without any file on the search path.
    std::string layout_name = zerr_max::kDefaultLayoutName;
    std::string layout_yaml = zerr_max::kDefaultLayoutYaml;
    if (offset >= 2) {
        if (atom_gettype(argv + 1) != A_SYM) {
            object_error((t_object*)x, "second argument must be a speaker layout name");
            return zerr_envelopes_fail(x);
        }
        const char* requested = atom_getsym(argv + 1)->s_name;
        layout_name           = zerr_envelopes_find_layout(x, requested);
        layout_yaml.clear();
        if (layout_name.empty()) {
            object_error((t_object*)x,
                         "cannot find speaker layout '%s' (tried as given, .yaml and .yml on "
                         "the search path)",
                         requested);
            return zerr_envelopes_fail(x);
        }
    }
    else {
        object_post((t_object*)x, "no speaker layout given, using the %s",
                    zerr_max::kDefaultLayoutName);
    }

    // Build the wrapper with Max's global rate and vector size; dsp64 re-sizes it for the
    // chain it ends up in. The Console printer is installed before initialize() so the
    // core's own load errors are visible.
    try {
        x->ze = new ZerrEnvelopes(sys_getsr(), sys_getblksize(), mode, layout_name, layout_yaml,
                                  zerr_max::consolePrinter((t_object*)x));
    }
    catch (const std::exception& e) {
        object_error((t_object*)x, "%s", e.what());
        return zerr_envelopes_fail(x);
    }

    std::string error;
    if (!x->ze->initialize(&error)) {
        object_error((t_object*)x, "%s", error.c_str());
        return zerr_envelopes_fail(x);
    }

    x->channel_count = x->ze->getOutputCount();

    attr_args_process(x, argc, argv);

    dsp_setup((t_pxobject*)x, x->ze->getInputCount());
    x->dsp_ready = true;

    outlet_new((t_object*)x, "multichannelsignal");

    return x;
}

void zerr_envelopes_free(t_zerr_envelopes* x)
{
    // dsp_free() must pair with dsp_setup(); a creation that failed earlier never ran it.
    if (x->dsp_ready) {
        dsp_free((t_pxobject*)x);
    }
    delete x->ze;
    x->ze = NULL;
}

//------------------------------------------------------------------------------
// UI and Info Methods
//------------------------------------------------------------------------------

void zerr_envelopes_assist(t_zerr_envelopes* x, void* b, long m, long a, char* s)
{
    if (m == ASSIST_INLET) {
        switch (a) {
        case 0:
            strcpy(s, "(signal) control: pulses reaching 1 jump (trigger), 0..1 position "
                      "(trajectory); also messages");
            break;
        case 1:
            strcpy(s, "(signal) spread 0..1, how far a jump spills onto neighbours (trigger "
                      "mode)");
            break;
        case 2:
            strcpy(s, "(signal) gain applied to every envelope");
            break;
        default:
            strcpy(s, "(signal)");
        }
    }
    else if (m == ASSIST_OUTLET) {
        snprintf(s, 256, "(multichannel signal) %ld envelopes, one per speaker in layout order",
                 x->channel_count);
    }
}

//------------------------------------------------------------------------------
// Multichannel Methods
//------------------------------------------------------------------------------

long zerr_envelopes_multichanneloutputs(t_zerr_envelopes* x, long outletindex)
{
    return x->channel_count;
}

//------------------------------------------------------------------------------
// DSP Methods
//------------------------------------------------------------------------------

void zerr_envelopes_dsp64(t_zerr_envelopes* x, t_object* dsp64, short* count, double samplerate,
                          long maxvectorsize, long flags)
{
    // The chain's own rate and vector size, which differ from the global ones inside a
    // resampled poly~ or after a change in Audio Status. prepare() keeps the speaker
    // state and is idempotent when nothing changed; on failure perform() stays silent.
    zerr::SystemConfigs cfgs;
    cfgs.sample_rate = (size_t)samplerate;
    cfgs.block_size  = (size_t)maxvectorsize;
    if (!x->ze->prepare(cfgs)) {
        object_error((t_object*)x, "cannot prepare for %ld samples at %.0f Hz; output muted",
                     maxvectorsize, samplerate);
    }

    dsp_add64(dsp64, (t_object*)x, (t_perfroutine64)zerr_envelopes_perform64, 0, NULL);
}

void zerr_envelopes_perform64(t_zerr_envelopes* x, t_object* dsp64, double** ins, long numins,
                              double** outs, long numouts, long sampleframes, long flags,
                              void* userparam)
{
    x->ze->perform(ins, numins, outs, numouts, sampleframes); // noexcept
}

//------------------------------------------------------------------------------
// Message Methods
//------------------------------------------------------------------------------

void zerr_envelopes_bang(t_zerr_envelopes* x)
{
    const auto& cfgs = x->ze->getSystemConfigs();
    object_post((t_object*)x,
                "zerr.envelopes~ %s: %s mode, %ld speakers from %s, block %ld, sr %ld",
                ZERR_MAX_VERSION, x->ze->getMode().c_str(), x->channel_count,
                x->ze->getLayoutName().c_str(), (long)cfgs.block_size, (long)cfgs.sample_rate);
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

    action      = atom_getsym(argv);
    index_count = argc - 1;

    indices = (int*)sysmem_newptr(index_count * sizeof(int));
    if (!indices) {
        object_error((t_object*)x, "active: out of memory");
        return;
    }

    for (i = 0; i < index_count; i++) {
        if (atom_gettype(argv + i + 1) == A_LONG) {
            indices[i] = (int)atom_getlong(argv + i + 1);
        }
        else if (atom_gettype(argv + i + 1) == A_FLOAT) {
            indices[i] = (int)atom_getfloat(argv + i + 1);
        }
        else {
            object_error((t_object*)x, "active: index %ld must be a number", i + 1);
            sysmem_freeptr(indices);
            return;
        }
    }

    x->ze->setActiveSpeakerIndexs(action->s_name, indices, index_count);

    sysmem_freeptr(indices);
}

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
        object_error((t_object*)x, "curr: not enough arguments");
        return;
    }

    if (atom_gettype(argv) == A_LONG) {
        speaker_index = atom_getlong(argv);
    }
    else if (atom_gettype(argv) == A_FLOAT) {
        speaker_index = (long)atom_getfloat(argv);
    }
    else {
        object_error((t_object*)x, "curr: argument must be a number");
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
        object_error((t_object*)x, "topo: not enough arguments");
        return;
    }

    if (atom_gettype(argv) != A_SYM) {
        object_error((t_object*)x, "topo: first argument must be a symbol");
        return;
    }

    action      = atom_getsym(argv);
    index_count = argc - 1;

    indices = (int*)sysmem_newptr(index_count * sizeof(int));
    if (!indices) {
        object_error((t_object*)x, "topo: out of memory");
        return;
    }

    for (i = 0; i < index_count; i++) {
        if (atom_gettype(argv + i + 1) == A_LONG) {
            indices[i] = (int)atom_getlong(argv + i + 1);
        }
        else if (atom_gettype(argv + i + 1) == A_FLOAT) {
            indices[i] = (int)atom_getfloat(argv + i + 1);
        }
        else {
            object_error((t_object*)x, "topo: index %ld must be a number", i + 1);
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

    if (argc < 1) {
        object_error((t_object*)x, "traj: expects at least one speaker number");
        return;
    }

    indices = (int*)sysmem_newptr(argc * sizeof(int));
    if (!indices) {
        object_error((t_object*)x, "traj: out of memory");
        return;
    }

    for (i = 0; i < argc; i++) {
        if (atom_gettype(argv + i) == A_LONG) {
            indices[i] = (int)atom_getlong(argv + i);
        }
        else if (atom_gettype(argv + i) == A_FLOAT) {
            indices[i] = (int)atom_getfloat(argv + i);
        }
        else {
            object_error((t_object*)x, "traj: index %ld must be a number", i + 1);
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
        object_error((t_object*)x, "interval: not enough arguments");
        return;
    }

    if (atom_gettype(argv) == A_FLOAT) {
        interval = atom_getfloat(argv);
    }
    else if (atom_gettype(argv) == A_LONG) {
        interval = (double)atom_getlong(argv);
    }
    else {
        object_error((t_object*)x, "interval: argument must be a number");
        return;
    }

    x->ze->setTriggerInterval((float)interval);
}

/**
 * @brief Method to print the current configuration parameters of the object.
 * @param x Pointer to the t_zerr_envelopes object.
 * @param s Unused symbol parameter.
 */
void zerr_envelopes_print(t_zerr_envelopes* x) { x->ze->printParameters(); }