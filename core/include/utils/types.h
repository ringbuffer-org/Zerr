/**
 * @file types.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief  Zerr* types definitions - Contains type definitions and structures used throughout the
 * Zerr library
 * @date 2024-02-06
 *
 * @copyright Copyright (c) 2023-2024
 */
#ifndef TYPES_H
#define TYPES_H
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "configs.h"

namespace zerr {
// basic types
using Sample = double; /**< Base type for audio sample values */
using Param  = float;  /**< Base type for parameter values used in audio processing */
using Index  = int;    /**< Base type for indexing and counting */

using Samples = std::vector<Sample>; /**< Vector container for audio samples */
using Blocks =
    std::vector<Samples>; /**< Collection of sample vectors for multi-channel processing */

struct AudioInputs {
    Samples block;            /**< Single block of audio samples for processing */
    Samples wave;             /**< Buffered audio frame for temporal analysis */
    std::vector<Sample> spec; /**< Spectral power data for frequency analysis */
}; /**< Consolidated structure for different types of audio input data */

using FeatureNames = std::vector<std::string>; /**< List of feature names to be processed */

/**< Envelope generation strategy */
enum class GenMode {
    Trigger,    /**< Jump to a new speaker on each trigger event */
    Trajectory, /**< Continuously move along a trajectory path */
};

/**< Speaker selection strategy within trigger mode */
enum class TriggerMode {
    Random, /**< Randomly select from connected speakers */
};

/**
 * @brief Parse a string into a GenMode enum value
 * @param s The string to parse ("trigger" or "trajectory")
 * @return GenMode The corresponding enum value
 * @throws std::invalid_argument if the string is not a valid mode
 */
inline GenMode parseGenMode(const std::string& s)
{
    if (s == "trigger")
        return GenMode::Trigger;
    if (s == "trajectory")
        return GenMode::Trajectory;
    throw std::invalid_argument("Unknown GenMode: " + s);
}

/**< Strategy for combining envelopes coming from several sources */
enum class CombMode {
    Add,  /**< Sum the envelopes across sources */
    Root, /**< Geometric mean: the Nth root of the N sources multiplied together */
    Max,  /**< Largest envelope value across sources */
};

/**
 * @brief Parse a string into a CombMode enum value without throwing
 * @param s The string to parse ("add", "root" or "max")
 * @param out Receives the parsed value; left untouched when the string is not a mode
 * @return true if the string named a mode
 *
 * The non-throwing counterpart of parseCombMode, for callers on a host's C callback
 * stack where an escaping exception would terminate the process.
 */
inline bool tryParseCombMode(const std::string& s, CombMode& out) noexcept
{
    if (s == "add") {
        out = CombMode::Add;
        return true;
    }
    if (s == "root") {
        out = CombMode::Root;
        return true;
    }
    if (s == "max") {
        out = CombMode::Max;
        return true;
    }
    return false;
}

/**
 * @brief Parse a string into a CombMode enum value
 * @param s The string to parse ("add", "root" or "max")
 * @return CombMode The corresponding enum value
 * @throws std::invalid_argument if the string is not a valid mode
 */
inline CombMode parseCombMode(const std::string& s)
{
    CombMode mode;
    if (!tryParseCombMode(s, mode)) {
        throw std::invalid_argument("Unknown CombMode: " + s);
    }
    return mode;
}

/**
 * @brief Render a CombMode as the string that names it
 */
inline const char* toString(CombMode mode) noexcept
{
    switch (mode) {
    case CombMode::Add:
        return "add";
    case CombMode::Root:
        return "root";
    case CombMode::Max:
        return "max";
    }
    return "max";
}

/**
 * @brief The supported combination modes as a human-readable list, for error messages
 *
 * Single source of truth for wrappers reporting a bad mode, so a new mode only has to be
 * added here and in tryParseCombMode.
 */
inline const char* combModeNames() noexcept { return "add, root or max"; }

using ConfigPath = std::string; /**< Path string for configuration files */

using Params = std::vector<Param>; /**< Vector container for parameter values */

using FeatureVal   = Param;  /**< Single feature value computed from an audio block */
using FeatureVals  = Params; /**< Collection of feature values at sample level or interpolated */
using FeaturesVals = std::vector<FeatureVals>; /**< Matrix of all extracted feature values */

// speaker configuration type
// coordinates
struct Cartesian {
    Param x; /**< X coordinate in Cartesian space */
    Param y; /**< Y coordinate in Cartesian space */
    Param z; /**< Z coordinate in Cartesian space */
};

struct Spherical {
    Param azimuth;   /**< Horizontal angle in spherical coordinates */
    Param elevation; /**< Vertical angle in spherical coordinates */
    Param distance;  /**< Distance from origin in spherical coordinates */
};

struct Position {
    Cartesian cartesian; /**< Position in Cartesian coordinate system */
    Spherical spherical; /**< Position in spherical coordinate system */
};

struct Orientation {
    Param yaw;   /**< Rotation around vertical axis */
    Param pitch; /**< Rotation around horizontal axis */
};

// specific configs
using Indexes = std::vector<Index>; /**< Collection of index values */
using TopoMatrix =
    std::map<Index, Indexes>; /**< Topology matrix mapping indices to their connections */
using SpeakerPair =
    std::pair<Index, Index>; /**< Pair of speaker indices for representing connections */

// system config
struct SystemConfigs {
    size_t sample_rate; /**< Audio sampling rate in Hz */
    size_t block_size;  /**< Size of processing blocks in samples */
};

} // namespace zerr
#endif // TYPES_H
