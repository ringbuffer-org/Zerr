/**
 * @file    mc.zerr.features_tilde.c
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   mc.zerr.features~ Max/MSP External
 * @date    2025-05-01
 *
 * @copyright  Copyright (c) 2023-2025
 * @license    MIT license
 */

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include "commonsyms.h"

// Object structure
typedef struct _zerr_features {
    t_pxobject x_obj;       // DSP object header
    long channel_count;     // Channel count for output
    t_bool fixed_channels;  // Whether channels are fixed or auto-adapting
    double *values;         // Storage for audio processing (if needed)
} t_zerr_features;

// Function prototypes
void *zerr_features_new(t_symbol *s, long argc, t_atom *argv);
void zerr_features_free(t_zerr_features *x);
void zerr_features_assist(t_zerr_features *x, void *b, long m, long a, char *s);
void zerr_features_dsp64(t_zerr_features *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void zerr_features_perform64(t_zerr_features *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
long zerr_features_multichanneloutputs(t_zerr_features *x, long outletindex);
long zerr_features_inputchanged(t_zerr_features *x, long index, long count);
void zerr_features_int(t_zerr_features *x, long n);
void zerr_features_float(t_zerr_features *x, double f);
void zerr_features_bang(t_zerr_features *x);
void zerr_features_channels(t_zerr_features *x, long num_channels);

// Global class pointer
static t_class *zerr_features_class = NULL;

//--------------------------------------------------------------------------

void ext_main(void *r)
{
    t_class *c;

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
    class_addmethod(c, (method)zerr_features_inputchanged, "inputchanged", A_CANT, 0);
    class_addmethod(c, (method)zerr_features_bang, "bang", 0);
    class_addmethod(c, (method)zerr_features_int, "int", A_LONG, 0);
    class_addmethod(c, (method)zerr_features_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)zerr_features_channels, "channels", A_LONG, 0);

    // Add attributes
    CLASS_ATTR_LONG(c, "chans", 0, t_zerr_features, channel_count);
    CLASS_ATTR_LABEL(c, "chans", 0, "Output Channels");
    CLASS_ATTR_FILTER_CLIP(c, "chans", 1, MC_MAX_CHANS);
    CLASS_ATTR_BASIC(c, "chans", 0);

    // Mark this class as having multichannel capabilities
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    zerr_features_class = c;
}

//--------------------------------------------------------------------------

void *zerr_features_new(t_symbol *s, long argc, t_atom *argv)
{
    t_zerr_features *x = (t_zerr_features *)object_alloc(zerr_features_class);
    long offset;

    if (x) {
        // Initialize default values
        x->channel_count = 8;  // Default to 8 channels
        x->fixed_channels = false;

        // Handle any construction arguments (before attributes)
        offset = attr_args_offset(argc, argv);
        if (offset >= 1) {
            x->channel_count = atom_getlong(argv);
            x->fixed_channels = true;  // If user provided a channel count, use fixed channels
        }

        // Ensure channel count is within bounds
        x->channel_count = CLAMP(x->channel_count, 1, MC_MAX_CHANS);

        // Allocate memory if needed
        x->values = (double *)sysmem_newptrclear(x->channel_count * sizeof(double));

        // Set up DSP
        dsp_setup((t_pxobject *)x, 1);  // One signal inlet
        
        // Mark as multichannel enabled
        x->x_obj.z_misc = Z_NO_INPLACE | Z_MC_INLETS;
        
        // Create multichannel outlet
        outlet_new((t_object *)x, "multichannelsignal");

        // Process attribute args
        attr_args_process(x, argc, argv);
        
        post("mc.zerr.features~: initialized with %ld channels", x->channel_count);
    }
    
    return x;
}

//--------------------------------------------------------------------------

void zerr_features_free(t_zerr_features *x)
{
    dsp_free((t_pxobject *)x);
    
    if (x->values) {
        sysmem_freeptr(x->values);
        x->values = NULL;
    }
}

//--------------------------------------------------------------------------

void zerr_features_assist(t_zerr_features *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_INLET) {
        sprintf(s, "(signal) Input signal");
    }
    else if (m == ASSIST_OUTLET) {
        sprintf(s, "(multichannel signal) Output signals");
    }
}

//--------------------------------------------------------------------------

long zerr_features_multichanneloutputs(t_zerr_features *x, long outletindex)
{
    // This is the key method that tells Max how many channels to allocate for this outlet
    post("multichanneloutputs called, returning %ld channels", x->channel_count);
    return x->channel_count;
}

//--------------------------------------------------------------------------

long zerr_features_inputchanged(t_zerr_features *x, long index, long count)
{
    // Return true if you want to adapt your output channel count based on input
    // Return false if you have a fixed number of output channels
    
    if (x->fixed_channels) {
        return false;  // We're not changing our output count
    }
    
    // If we're auto-adapting, we could use the input count to determine output
    // For now just keeping our fixed channel count
    return false;
}

//--------------------------------------------------------------------------

void zerr_features_dsp64(t_zerr_features *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    // Add the perform routine to the DSP chain
    dsp_add64(dsp64, (t_object *)x, (t_perfroutine64)zerr_features_perform64, 0, NULL);
    post("dsp64 setup complete");
}

//--------------------------------------------------------------------------

void zerr_features_perform64(t_zerr_features *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    // Simple implementation - just clear all outputs
    for (long channel = 0; channel < numouts; channel++) {
        if (channel < x->channel_count) {
            // Clear the output buffer
            memset(outs[channel], 0, sampleframes * sizeof(double));
        }
    }
}

//--------------------------------------------------------------------------

void zerr_features_bang(t_zerr_features *x)
{
    post("mc.zerr.features~: current channel count = %ld", x->channel_count);
}

//--------------------------------------------------------------------------

void zerr_features_int(t_zerr_features *x, long n)
{
    zerr_features_channels(x, n);
}

//--------------------------------------------------------------------------

void zerr_features_float(t_zerr_features *x, double f)
{
    zerr_features_channels(x, (long)f);
}

//--------------------------------------------------------------------------

void zerr_features_channels(t_zerr_features *x, long num_channels)
{
    // Ensure within bounds
    num_channels = CLAMP(num_channels, 1, MC_MAX_CHANS);
    
    // Only update if needed
    if (x->channel_count != num_channels) {
        x->channel_count = num_channels;
        x->fixed_channels = true;  // User explicitly set channels
        
        // Reallocate memory if needed
        if (x->values) {
            x->values = (double *)sysmem_resizeptr(x->values, x->channel_count * sizeof(double));
        }
        
        post("mc.zerr.features~: channel count updated to %ld", x->channel_count);
    }
}