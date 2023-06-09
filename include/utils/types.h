#ifndef TYPES_H
#define TYPES_H

#include "configs.h"

namespace zerr {

// audio types
#ifdef PUREDATA
    typedef float t_sample; //puredata still only support 16bit
#else
    typedef double t_sample; 
#endif

typedef std::vector<t_sample> t_samples; 

typedef t_samples t_blockIn;
typedef t_samples t_blockOut;

typedef std::vector<t_blockIn>  t_blockIns;
typedef std::vector<t_blockOut> t_blockOuts;

typedef t_samples t_audioBuf;

// feature types
typedef std::string t_featureName;

typedef std::vector<t_featureName> t_featureNameList; 

typedef float t_value;

typedef struct {
    t_value original;
    t_value normalized;
} t_featureValue;

typedef std::vector<t_featureValue> t_featureValueList;

typedef t_value t_trigger;  // 0,1
typedef t_value t_category; // 0,1,2,3,4
typedef t_value t_floating; // float

typedef t_value t_volume;
typedef std::vector<t_volume> t_volumes;

// spectrum types
// #ifdef PUREDATA
//     typedef float t_bin; //puredata still only support 16bit
// #else
//     typedef double t_bin; 
// #endif

typedef struct {
    t_sample real;
    t_sample img;
} t_complex;

typedef std::vector<t_complex> t_fftBuf;
typedef std::vector<t_sample> t_specBuf;

typedef struct {
    t_blockIn  blck;
    t_audioBuf wave;
    t_specBuf  spec;
} t_featureInputs ;

// configs
#define PI 3.14159265

#define AUDIO_BUFFER_SIZE 1024 

#define VOLUME_THRESHOLD  0.01


typedef struct {
    size_t sample_rate;
    size_t block_size;
}t_systemConfigs;

} // namespace zerr
#endif //TYPES_H




