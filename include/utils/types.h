#ifndef TYPES_H
#define TYPES_H

#include "configs.h"

namespace zerr {

// audio types
typedef double t_sample; /**< type of single audio sample */

typedef std::vector<t_sample> t_samples; /**< basic type of all kinds of audio vector */

typedef t_samples t_blockIn;  /**< input audio block */
typedef t_samples t_blockOut; /**< output audio block */
typedef t_samples t_audioBuf; /**< buffered audio frame */

typedef std::vector<t_blockIn>  t_blockIns;  /**< multi-channel input audio blocks: unused */
typedef std::vector<t_blockOut> t_blockOuts; /**< multi-channel output audio blocks: unused? */
typedef std::vector<t_audioBuf> t_audioBufs; /**< multi-channel audio buffer: unused */

// spectral types
typedef struct {
    t_sample real;
    t_sample img;
} t_complex; /**< complex number */

typedef std::vector<t_complex> t_fftBuf; /**< complex buffer for fft output */
typedef std::vector<t_sample> t_specBuf; /**< spectral power buffer */

typedef struct {
    t_blockIn  blck; /**< single audio block */
    t_audioBuf wave; /**< buffered audio frame */
    t_specBuf  spec; /**< spectral power */
} t_featureInputs; /**< structure for different feature extractor input data*/


// feature types
typedef std::string t_featureName; /**< Identification name of the feature */
typedef std::vector<t_featureName> t_featureNameList; /**< The list of feature name to be activated */

typedef float t_value; /**< base type of all non-audio value*/
typedef std::vector<t_value> t_values;   /**< base type of all non-audio vector */


typedef t_value t_featureValue; /**< feature value calculated via block */
typedef t_values t_featureBuf;   /**< feature value in sample level or after interpolation */
typedef std::vector<t_featureBuf> t_featureValueList; /**< list of all extracted features */


typedef t_value t_volume;
typedef std::vector<t_volume> t_volumes;


// structure
typedef struct {
    size_t sample_rate;
    size_t block_size;
}t_systemConfigs;

} // namespace zerr
#endif //TYPES_H




