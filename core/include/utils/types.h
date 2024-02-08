/**
 * @file types.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief  Zerr* types definitions
 * @date 2024-02-06
 * 
 * @copyright Copyright (c) 2023-2024
 */
#ifndef TYPES_H
#define TYPES_H
#include <vector>
#include <string>
#include <map>

#include "configs.h"

namespace zerr {
// basic types
using Sample = double;      /**< base type of a sample value */
using Param  = float;       /**< base type of a parameter value */
using Index  = int;         /**< base type of an index value */
// using zString   = std::string; /**< base type of an name identifiers */

struct Complex{
    Sample real;
    Sample img;
}; /**< complex number */

using Samples = std::vector<Sample>; /**< basic type of all kinds of audio vector */

using Block = Samples;  /**< audio block */
using Blocks = std::vector<Block>;  /**< multi-channel audio blocks */

using AudioBuffer = Samples;  /**< buffered audio frame */
using AudioBuffers = std::vector<AudioBuffer>;  /**< multi-channel audio buffer */

using FFTBuffer = std::vector<Complex>;  /**< complex buffer for fft output */
using SpecBuffer = std::vector<Sample>;  /**< spectral power buffer */

struct AudioInputs{
    Block      block; /**< single audio block */
    AudioBuffer wave; /**< buffered audio frame */
    SpecBuffer  spec; /**< spectral power */
};  /**< structure for different feature extractor input data*/

using FeatureName = std::string; /**< Identification name of the feature */
using FeatureNames = std::vector<FeatureName>; /**< The list of feature name to be activated */

using Mode = std::string;
using ConfigPath = std::string;

// TODO(Zeyu Yang): If this only use in PD wrapper, move it out
typedef struct {
    char **names;  // Pointer to the list of strings
    int num;     // Number of strings in the list
} t_featureNames;

using Params = std::vector<Param>; /**< base type of all non-audio vector */

using FeatureVal  = Param;  /**< feature value calculated via block */
using FeatureVals = Params; /**< feature value in sample level or after interpolation */
using FeaturesVals = std::vector<FeatureVals>; /**< list of all extracted features */

// speaker configuration type
// coordinates
struct Cartesian{
    Param x;
    Param y;
    Param z;
};

struct Spherical{
    Param azimuth;
    Param elevation;
    Param distance;
};

struct Position{
    Cartesian cartesian;
    Spherical spherical;
};

struct Orientation{
    Param yaw;
    Param pitch;
};

// specific configs
using Indexes = std::vector<Index>;
using TopoMatrix = std::map<Index, Indexes>;
using Pair = std::pair<Index, Index>;

// system config
typedef struct {
    size_t sample_rate;
    size_t block_size;
} SystemConfigs;

} // namespace zerr
#endif //TYPES_H




