#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include "utils.h"

namespace zerr {
/**
* Virtual Class that defines the basic structure of Feature extractor
*/
class FeatureExtractor {
public: 
    /**
    * Virtual desctructor for dealing with virtual methods
    */
    virtual ~FeatureExtractor(){};
    /**
    * get functions
    * basic info(name, category, description) can be accessed before 
    */
    virtual std::string get_name() = 0;
    virtual std::string get_category() = 0;
    virtual std::string get_description() = 0;
    /**
    * initialize the feature extraction algorithm 
    */
    virtual void initialize(SystemConfigs sys_cfg) = 0;
    /**
    * Run algorithm on the signal in the input buffer and update the output buffer
    */
    virtual void extract() = 0;
    /**
    * Run algorithm on the signal in the input buffer and update the output buffer
    */
    virtual void reset() = 0;
    /**
    * fetch samples from audio stream and load to the input buffer
    */
    virtual void fetch(t_featureInputs x) = 0;
    /**
    * send the process results back
    */
    virtual t_featureBuf send() = 0;
    /**
    * statue polling about if the feature extractor is initialized
    */
    bool is_initialized(){return initialized;}
    void set_initialize_statue(bool s){initialized=s;}

protected:
    Samples       x; /**< input data in t_samples type, the base type of t_blockIn, t_audioBuf, t_specBuf*/
    t_featureBuf    y; /**< output data */

    SystemConfigs system_configs;
    bool initialized=false; // 
}; // Class FeatureExtractor

}      // Namespace zerr 
#endif // FEATUREEXTRACTOR_H

