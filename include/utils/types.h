#ifndef TYPES_H
#define TYPES_H

namespace zerr {

// audio types
#ifdef PUREDATA
    typedef float t_sample; //puredata still only support 16bit
#else
    typedef double t_sample; 
#endif

typedef std::vector<t_sample> t_samples; 

typedef t_samples t_blockIn;

typedef t_samples t_audioBuf;

// feature types
typedef std::string t_featureName;

typedef std::vector<t_featureName> t_featureList; 

typedef float t_featureVal;
typedef std::vector<t_featureVal> t_featureVals;

//spectrum types
#ifdef PUREDATA
    typedef float t_bin; //puredata still only support 16bit
#else
    typedef double t_bin; 
#endif

typedef std::vector<t_bin> specBuf;

// typedef t_value t_trigger;
// typedef t_value t_category;
// typedef t_value t_floating;

//processing_mode
// #define zSAMPLE 1
// #define zBLOCK 32
// #define zFRAME 2048 

// output_mode
// typedef float FLOATING_POINTS; // :)
// typedef unsigned int CATEGORY_INDEX;
// typedef enum { On = 1, Off = 0 } TRIGGER_BANG;
}

#endif //TYPES_H




