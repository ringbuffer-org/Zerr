/**
 * @file    mc.zerr.features_tilde.cpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   mc.zerr.features~ Max/MSP External
 * @date    2025-05-01
 *
 * @copyright  Copyright (c) 2023-2025
 * @license    MIT license
 */

#include "c74_min.h"
#include <unordered_map>

using namespace c74::min;

// Forward declare the class first
class mc_zerr_features_tilde;

long mc_zerr_features_tilde_mc_outputs(mc_zerr_features_tilde *x, long outletindex);
long mc_zerr_features_tilde_inputchanged(mc_zerr_features_tilde *x, long index, long count);

class mc_zerr_features_tilde : public object<mc_zerr_features_tilde>, public mc_operator<> {
 public:
    // ==========================
    // Basic informations
    // ==========================
    MIN_DESCRIPTION { "[mc.zerr.features~] xxxx"
                      "[mc.zerr.features~] xxxx"
                      "xxxxxxxxxxx" };
    MIN_TAGS { "DSP" };
    MIN_AUTHOR { "Zeyu Yang" };
    MIN_RELATED { "mc.zerr.disperser~, mc.zerr.combinator~, mc.zerr.generator~" };

    // ==========================
    // Constructor
    // ==========================
    mc_zerr_features_tilde(const atoms& args = {})
    {
        if (args.size() > 0) {
            channel = args[0];
        }

        cout << "Constructor: channel = " << channel << " (this=" << this << ")" << endl;
    }

    // ==========================
    // Inlets & Outlets
    // ==========================
    inlet<> m_inlet { this, "(signal) input source signal", "signal" };
    outlet<> m_outlet { this, "(multichannelsignal) output signals for each loudspeaker", "multichannelsignal" };

    // ===================================
    // Max class setup with native Max API
    // ===================================
    // long _mc_outputs(c74::max::t_object* obj, long outletindex)
    // {
    //     return 7;
    // }

    // Specifying multichannel outlet channel counts
    message<> maxclass_setup {
        this, "maxclass_setup",
        [this](const atoms& args, const int inlet) -> atoms {
            c74::max::t_class* c = args[0];

            // Register the multichannel outputs method
            class_addmethod(c, (c74::max::method)mc_zerr_features_tilde_mc_outputs,
                "multichanneloutputs", c74::max::A_CANT, 0);

            class_addmethod(c, (c74::max::method)mc_zerr_features_tilde_inputchanged, 
                "inputchanged", c74::max::A_CANT, 0);

            return {};
        }
    };

    // ==========================
    // Multi-Channel Operation
    // ==========================
    void operator()(audio_bundle input, audio_bundle output)
    {
        // Clear output - placeholder for actual signal processing
        output.clear();
        return;
    }

    // ==========================
    // Messages
    // ==========================
    // Respond to bang message
    message<> bang { this, "bang", "Post current channel count.",
        [this](const atoms& args, const int inlet) -> atoms {
            cout << "Current channel count: " << channel << endl;
            return {};
        } };

    // Add a method to manually set the channel count
    message<> ch { this, "ch", "Set the number of output channels",
        [this](const atoms& args, const int inlet) -> atoms {
            if (args.size() > 0) {
                // Update the instance value
                channel = args[0];

                cout << "Channel count updated to: " << channel << " for instance " << this << endl;
            }
            return {};
        } };

    // ==========================
    // Attributes & Variables
    // ==========================
    // Simple member variable for channel count
    // int channel=3;
    attribute<int> channel { this, "channels", 8 };
};

long mc_zerr_features_tilde_mc_outputs(mc_zerr_features_tilde *x, long outletindex)
{
    return x->channel == 0 ? 5 : 9;
}

long mc_zerr_features_tilde_inputchanged(mc_zerr_features_tilde *x, long index, long count)
{
    return true;
}

// Create the Max external
MIN_EXTERNAL(mc_zerr_features_tilde);