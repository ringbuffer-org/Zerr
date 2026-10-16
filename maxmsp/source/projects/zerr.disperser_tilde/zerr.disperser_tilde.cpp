/**
 * @file    zerr.disperser_tilde.cpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   zerr.disperser~ Max/MSP External
 * @date    2025-04-29
 *
 * @copyright  Copyright (c) 2023-2025
 * @license    MIT license
 */

#include "c74_min.h"

using namespace c74::min;

class zerr_disperser_tilde : public object<zerr_disperser_tilde>, public mc_operator<> {
  public:
    // ==========================
    // Basic informations
    // ==========================
    MIN_DESCRIPTION{"zerr.disperser~ multiplies the source signal with the envelopes to achieve "
                    "multichannel panning."};
    MIN_TAGS{"zerr"};
    MIN_AUTHOR{"Zeyu Yang"};
    MIN_RELATED{"zerr.envelopes~, zerr.combinator~, zerr.features~"};
    // The reference page is hand-written in maxmsp/docs/; keep Min from regenerating it.
    MIN_FLAGS{documentation_flags::do_not_generate};

    // ==========================
    // Inlets & Outlets
    // ==========================
    inlet<> m_inlet_src{this, "(signal) mono source signal", "signal"};
    inlet<> m_inlet_envs{this,
                         "(multichannelsignal) envelopes from zerr.envelopes~ or zerr.combinator~",
                         "multichannelsignal"};
    outlet<> m_outlet{
        this,
        "(multichannelsignal) source multiplied by each envelope, one channel per loudspeaker",
        "multichannelsignal"};

    // ==========================
    // Constructor
    // ==========================
    zerr_disperser_tilde(const atoms& args = {}) {}

    // ===================================
    // Max class setup with native Max API
    // ===================================
    // Max hands these callbacks the t_object*, which for a Min external is the
    // minwrap<T>: a Max object header followed by the C++ instance. Casting the
    // t_object* straight to zerr_disperser_tilde* would address the header, so every
    // read and write of the channel counts below would land in Max's own fields.
    // wrapper_find_self returns the minwrap; m_min_object is this class.
    static zerr_disperser_tilde& _self(c74::max::t_object* obj)
    {
        return wrapper_find_self<zerr_disperser_tilde>(obj)->m_min_object;
    }

    static long _mc_outputs(c74::max::t_object* obj, long outletindex)
    {
        auto& x = _self(obj);

        // The source must be mono. With anything else the outlet drops to one
        // channel and the operator below outputs silence.
        if (x.channelCountSrc != 1) {
            return 1;
        }

        return x.channelCountEnv;
    }

    static long _input_changed(c74::max::t_object* obj, long inletindex, long count)
    {
        // An unconnected inlet reports a count of 1, like a mono signal.
        auto& x = _self(obj);

        if (inletindex == 0) {
            x.channelCountSrc = count;
            // Say it once, on the main thread, when the source turns multichannel;
            // the operator stays silent because it runs on the audio thread.
            if (count != 1 && !x.srcWarned) {
                x.cerr << "source inlet expects a mono signal (got " << count
                       << " channels); output muted until it is mono again" << endl;
                x.srcWarned = true;
            }
            else if (count == 1) {
                x.srcWarned = false;
            }
            // The outlet count depends on the source too, so ask Max to re-query it.
            return true;
        }

        if (inletindex == 1) {
            x.channelCountEnv = count;
            return true;
        }

        return false;
    }

    // Specifying multichannel outlet channel counts and auto-adapt to signal chain change
    // Ref: https://sdk.cdn.cycling74.com/max-sdk-8.2.0/chapter_mc.html
    message<> maxclass_setup{
        this, "maxclass_setup",
        // It is usually recommended to use the MIN_FUNCTION macro for proper lambda prototype
        // However, it can breack my linter(maybe), so boilerplate is used here
        [this](const atoms& args, const int inlet) -> atoms {
            c74::max::t_class* c = args[0];

            class_addmethod(c, (c74::max::method)&_mc_outputs, "multichanneloutputs",
                            c74::max::A_CANT, 0);

            class_addmethod(c, (c74::max::method)&_input_changed, "inputchanged", c74::max::A_CANT,
                            0);

            return {};
        }};

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
    int channelCountSrc{1};
    int channelCountEnv{1};
    bool srcWarned{false}; ///< the multichannel-source error was posted for the current state
};

MIN_EXTERNAL(zerr_disperser_tilde);
