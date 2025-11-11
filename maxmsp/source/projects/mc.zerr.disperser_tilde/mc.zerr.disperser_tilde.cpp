/**
 * @file    mc.zerr.disperser_tilde.cpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   mc.zerr.disperser~ Max/MSP External
 * @date    2025-04-29
 *
 * @copyright  Copyright (c) 2023-2025
 * @license    MIT license
 */

#include "c74_min.h"

using namespace c74::min;

class mc_zerr_disperser_tilde : public object<mc_zerr_disperser_tilde>, public mc_operator<> {
 public:
    // ==========================
    // Basic informations
    // ==========================
    MIN_DESCRIPTION { "The [zerr_disperser~] is for simply multiply the source signal with the envelopes to achieve multichannel panning." };
    MIN_TAGS { "zerr" };
    MIN_AUTHOR { "Zeyu Yang" };
    MIN_RELATED { "zerr.disperser~, zerr.combinator~, mc.zerr.combinator~, mc.disperser~" };

    // ==========================
    // Inlets & Outlets
    // ==========================
    inlet<> m_inlet_src { this, "(signal) input source signal", "signal" };
    inlet<> m_inlet_envs { this, "(multichannelsignal) input envelopes from the generator/combinator", "signal" };
    outlet<> m_outlet { this, "(multichannelsignal) output signals for each loudspeaker", "multichannelsignal" };

    // ==========================
    // Constructor
    // ==========================
    mc_zerr_disperser_tilde(const atoms& args = {})
        : channelCountEnv { 1 }
        , channelCountSrc { 1 }
    {
    }

    // ===================================
    // Max class setup with native Max API
    // ===================================
    static long _mc_outputs(c74::max::t_object* obj, long outletindex)
    {
        auto* x = reinterpret_cast<mc_zerr_disperser_tilde*>(obj);

        // The source signal must be 1-channel
        // Force the outlet to 1-channel when invalid source input connected
        // The output will be all zero according to the channel check in the operator
        if (x->channelCountSrc != 1) {
            return 1;
        }

        return x->channelCountEnv;
    }

    static long _input_changed(c74::max::t_object* obj, long inletindex, long count)
    {
        // Something unintuitive. If there is any unconnected inlet, all inlet count will be one.
        auto* x = reinterpret_cast<mc_zerr_disperser_tilde*>(obj);

        // Store the channel count of the source inlet
        // We don't react to the source inlet change
        if (inletindex == 0) {
            x->channelCountSrc = count;
            return false;
        }

        // Store the channel count of the envelope inlet
        if (inletindex == 1) {
            x->channelCountEnv = count;
            return true;
        }

        return false; // There is no other options
    }

    // Specifying multichannel outlet channel counts and auto-adapt to signal chain change
    // Ref: https://sdk.cdn.cycling74.com/max-sdk-8.2.0/chapter_mc.html
    message<> maxclass_setup {
        this, "maxclass_setup",
        // It is usually recommended to use the MIN_FUNCTION macro for proper lambda prototype
        // However, it can breack my linter(maybe), so boilerplate is used here
        [this](const atoms& args, const int inlet) -> atoms {
            c74::max::t_class* c = args[0];

            class_addmethod(c, (c74::max::method)&_mc_outputs,
                "multichanneloutputs", c74::max::A_CANT, 0);

            class_addmethod(c, (c74::max::method)&_input_changed,
                "inputchanged", c74::max::A_CANT, 0);

            return {};
        }
    };

    // ==========================
    // Multi-Channel Operation
    // ==========================
    void operator()(audio_bundle input, audio_bundle output)
    {
        // Check the channel structure.
        if (input.channel_count() != output.channel_count() + 1) {
            output.clear();
            return;
        }

        // get the pointer to the source signal (signal channel)
        auto source = input.samples(0);

        // simply apply envelopes to the source signal
        for (auto i = 0; i < input.frame_count(); ++i) {
            for (auto channel = 1; channel < input.channel_count(); ++channel) {
                output.samples(channel - 1)[i] = input.samples(channel)[i] * source[i];
            }
        }
    }

    // ==========================
    // Messages
    // ==========================
    // Disable bang message
    // message<> bang { this, "bang", "Ignored bang message.",
    //     [this](const atoms& args, const int inlet) -> atoms {
    //         return {};
    //     } };

 private:
    int channelCountSrc { 1 };
    int channelCountEnv { 1 };
};

MIN_EXTERNAL(mc_zerr_disperser_tilde);
