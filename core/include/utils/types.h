/**
 * @file types.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief  Zerr* type definitions
 * @date 2023-08-09
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef TYPES_H
#define TYPES_H
#include <vector>
#include <string>
#include <map>

#include "configs.h"


namespace zerr {
// basic types
using Sample = double;  /**< base type of a sample value */
using Param  = float;   /**< base type of a parameter value */
using Index  = int;     /**< base type of an index value */

typedef struct {
    Sample real;
    Sample img;
} Complex; /**< complex number */

using Samples = std::vector<Sample>; /**< basic type of all kinds of audio vector */

using Block = Samples;  /**< audio block */

typedef Samples t_audioBuf; /**< buffered audio frame */

typedef std::vector<Block>  Blocks;  /**< multi-channel audio blocks */
typedef std::vector<t_audioBuf> t_audioBufs; /**< multi-channel audio buffer */

// spectral types
typedef std::vector<Complex> t_fftBuf; /**< complex buffer for fft output */
typedef std::vector<Sample> t_specBuf; /**< spectral power buffer */

typedef struct {
    Block  block; /**< single audio block */
    t_audioBuf wave; /**< buffered audio frame */
    t_specBuf  spec; /**< spectral power */
} t_featureInputs;   /**< structure for different feature extractor input data*/

// feature types
typedef std::string FeatureName; /**< Identification name of the feature */
typedef std::vector<FeatureName> t_featureNameList; /**< The list of feature name to be activated */


// TODO(Zeyu Yang): If this only use in PD wrapper, move it out
typedef struct {
    char **names;  // Pointer to the list of strings
    int num;     // Number of strings in the list
} t_featureNames;


typedef std::vector<Param> t_values; /**< base type of all non-audio vector */

typedef Sample t_featureValue;  /**< feature value calculated via block */
typedef Samples t_featureBuf;   /**< feature value in sample level or after interpolation */
typedef std::vector<t_featureBuf> t_featureValueList; /**< list of all extracted features */

// speaker configuration type
// coordinates
typedef struct {
    Param x;
    Param y;
    Param z;
} Cartesian;

typedef struct {
    Param azimuth;
    Param elevation;
    Param distance;
} Spherical;

typedef struct {
    Cartesian cartesian;
    Spherical spherical;
} t_position;

typedef struct {
    Param yaw;
    Param pitch;
} t_orientation;

// specific configs
typedef std::vector<Index> Indexes;
typedef std::map<Index, Indexes> TopoMatrix;

typedef std::pair<Index, Index> t_pair;

// system config type
typedef struct {
    size_t sample_rate;
    size_t block_size;
}SystemConfigs;

enum SelectionMode {
    trigger,
    trajectory
};


} // namespace zerr
#endif //TYPES_H




