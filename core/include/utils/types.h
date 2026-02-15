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
#include <string>
#include <vector>

#include "configs.h"

namespace zerr {
// basic types
using Sample = double; /**< Base type for audio sample values */
using Param  = float;  /**< Base type for parameter values used in audio processing */
using Index  = int;    /**< Base type for indexing and counting */

struct Complex {
    Sample real; /**< Real component of complex number */
    Sample img;  /**< Imaginary component of complex number */
}; /**< Complex number representation for frequency domain calculations */

using Samples = std::vector<Sample>; /**< Vector container for audio samples */

using Block  = Samples;            /**< Single block of audio samples */
using Blocks = std::vector<Block>; /**< Collection of audio blocks for multi-channel processing */

using AudioBuffer = Samples; /**< Buffer for storing audio frames */
using AudioBuffers =
    std::vector<AudioBuffer>; /**< Collection of audio buffers for multi-channel storage */

using FFTBuffer  = std::vector<Complex>; /**< Buffer for storing FFT results as complex numbers */
using SpecBuffer = std::vector<Sample>;  /**< Buffer for storing spectral power values */

struct AudioInputs {
    Block block;      /**< Single block of audio samples for processing */
    AudioBuffer wave; /**< Buffered audio frame for temporal analysis */
    SpecBuffer spec;  /**< Spectral power data for frequency analysis */
}; /**< Consolidated structure for different types of audio input data */

using FeatureName  = std::string;              /**< String identifier for audio features */
using FeatureNames = std::vector<FeatureName>; /**< List of feature names to be processed */

using Mode       = std::string; /**< String identifier for processing modes */
using ConfigPath = std::string; /**< Path string for configuration files */

// TODO(Zeyu Yang): If this only use in PD wrapper, move it out
struct t_featureNames {
    char** names; /**< Array of feature name strings */
    int num;      /**< Number of feature names in the array */
};

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
    std::map<Index, Indexes>;         /**< Topology matrix mapping indices to their connections */
using Pair = std::pair<Index, Index>; /**< Pair of indices for representing connections */

// system config
struct SystemConfigs {
    size_t sample_rate; /**< Audio sampling rate in Hz */
    size_t block_size;  /**< Size of processing blocks in samples */
};

} // namespace zerr
#endif // TYPES_H
