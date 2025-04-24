/// @file
/// @ingroup    zerr
/// @copyright  Copyright 2023-2025
/// @license

#include "c74_min.h"

using namespace c74::min;

class mc_zerr_disperser_tilde : public object<mc_zerr_disperser_tilde>, public mc_operator<> {
 public:
    MIN_DESCRIPTION { "Apply envelope signals to source signal"
                      "[mc.zerr.disperser~] provides information about what is happening in the multi-channel signal "
                      "with sample accuracy." };
    MIN_TAGS { "dsp" };
    MIN_AUTHOR { "Zeyu Yang" };
    MIN_RELATED { "zerr.disperser~, zerr.combinator~" };

    inlet<> m_inlet_src { this, "(signal) input source signal", "signal" };
    inlet<> m_inlet_envs { this, "(multichannelsignal) input envelopes from the generator/combinator" };

    outlet<> m_outlet { this, "(multichannelsignal) output signals for each loudspeaker" };

    void operator()(audio_bundle input, audio_bundle output)
    {

        for (auto i = 0; i < input.frame_count(); ++i) {
            for (auto channel = 0; channel < input.channel_count(); ++channel) {
                output.samples(channel)[i] = input.samples(channel)[i];
            }
        }
    }
};

MIN_EXTERNAL(mc_zerr_disperser_tilde);
