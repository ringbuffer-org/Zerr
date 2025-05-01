/**
 * @file    mc.zerr.features_tilde.cpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   mc.zerr.features~ Max/MSP External
 * @date    2025-04-29
 *
 * @copyright  Copyright (c) 2023-2025
 * @license    MIT license
 */

#include "c74_min.h"

using namespace c74::min;

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
    // Attributes
    // ==========================
    int channel;
    // attribute<int> channel {
    //     this,
    //     "channel",
    //     16, // Default value
    //     title { "Channel Count" },
    //     description { "Number of output channels" },
    // };

    // // define an optional argument for setting the message
    // argument<int> channel_arg { this, "channel", "Initial value for the channel attribute.",
    //     [this](const c74::min::atom& arg) {
    //         channel = arg;
    //     } };

    // define an optional argument for setting the message
    argument<symbol> greeting_arg { this, "greeting", "Initial value for the greeting attribute.",
        [this](const c74::min::atom& arg) {
            greeting = arg;
        } };

    // the actual attribute for the message
    attribute<symbol> greeting { this, "greeting", "hello world",
        description {
            "Greeting to be posted. "
            "The greeting will be posted to the Max console when a bang is received." } };

    // ==========================
    // Constructor
    // ==========================
    mc_zerr_features_tilde(const atoms& args = {})
    {
        channel = 16;

        if (args.size() > 0) {
            channel = args[0];
        }

        cout << "Constructor: channel = " << channel << " (this=" << this << ")" << endl;
    }

    // ==========================
    // Inlets & Outlets
    // ==========================
    inlet<> m_inlet { this, "(signal) input source signal", "signal" }; // "signal" is useless here, just as reminder

    outlet<> m_outlet { this, "(multichannelsignal) output signals for each loudspeaker", "multichannelsignal" };

    // clone(const atoms& args = {})
    // {
    //     if (args.empty())
    //         error("argument required");

    //     auto inlet_count = args[0];
    //     auto outlet_count = inlet_count * 2;

    //     for (auto i = 0; i < inlet_count; ++i) {
    //         auto an_inlet = std::make_unique<inlet<>>(this, "(bang) my assist message");
    //         m_inlets.push_back(std::move(an_inlet));
    //     }

    //     for (auto i = 0; i < outlet_count; ++i) {
    //         auto an_outlet = std::make_unique<outlet<>>(this, "my outlet assist message");
    //         m_outlets.push_back(std::move(an_outlet));
    //     }
    // }

    // ===================================
    // Max class setup with native Max API
    // ===================================

    // static long _mc_outputs(mc_zerr_features_tilde* x, long outletindex)
    // {
    //     // auto* x = reinterpret_cast<mc_zerr_features_tilde*>(obj);

    //     c74::max::post("_mc_outputs: x=%p, channelCountFt=%d", x, x->channelCountFt);

    //     return x->channelCountFt;
    // }

    static long _mc_outputs(c74::max::t_object* obj, long outletindex)
    {
        // auto* x = reinterpret_cast<mc_zerr_features_tilde*>(obj);
        mc_zerr_features_tilde* x = (mc_zerr_features_tilde*)obj;
        // Access the attribute value instead of member variable directly
        int count = x->channel;

        c74::max::post("_mc_outputs(%p): returning %d channels", obj, count);

        return count;
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

            return {};
        }
    };

    // Special setup method that runs during instantiation
    // message<> setup { this, "setup",
    //     [this](const atoms& args, const int inlet) -> atoms {
    //         // Initialize with default values

    //         // Use constructor arguments if provided
    //         if (args.size() > 0) {
    //             if (args[0].a_type == c74::max::A_LONG || args[0].a_type == c74::max::A_FLOAT)
    //                 channelCountFt = args[0];
    //         }

    //         c74::max::post("Setup method: channelCountFt set to %d (this=%p)", channelCountFt, this);
    //         return {};
    //     }
    // };

    // ==========================
    // Multi-Channel Operation
    // ==========================
    void operator()(audio_bundle input, audio_bundle output)
    {
        output.clear();
        return;
    }

    // ==========================
    // Messages
    // ==========================
    // Disable bang message
    // message<> bang { this, "bang", "Post something to the Max console.",
    //     [this](const atoms& args, const int inlet) -> atoms {
    //         cout << "channelCountFt: " << channelCountFt << endl;
    //         return {};
    //     } };
    // respond to the bang message to do something
    message<> bang { this, "bang", "Post the greeting.",
        [this](const atoms& args, const int inlet) -> atoms {
            symbol the_greeting = greeting; // fetch the symbol itself from the attribute named greeting

            cout << the_greeting << endl; // post to the max console
            return {};
        } };

    // Disable number message
    message<> number { this, "number",
        [this](const atoms& args, const int inlet) -> atoms {
            return {};
        } };

    // private:
    //    int channelCountFt; // How many channel of features are generated together
};

MIN_EXTERNAL(mc_zerr_features_tilde);
