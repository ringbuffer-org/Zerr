#ifndef CONFIGS_H
#define CONFIGS_H

#include <cstddef>

// #define TESTMODE /**< Print log when TESTMODE Macro defined */

namespace zerr {

/// Threshold value for triggering feature extraction
inline constexpr double TRIGGER_THRESHOLD = 1e-4;

/// Pi constant (lowercase to avoid macro clash with Max/MSP SDK)
inline constexpr double pi = 3.14159265;

/// Size of audio processing buffer in samples
inline constexpr std::size_t AUDIO_BUFFER_SIZE = 2048;

/// Minimum volume threshold for audio processing
inline constexpr double VOLUME_THRESHOLD = 1e-4;

/// Scaling factor for distance calculations in speaker positioning
inline constexpr double DISTANCE_SCALE = 1e-1;

} // namespace zerr

#endif // CONFIGS_H
