#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include "utils.h"

namespace zerr {
/**
* Virtual Class that defines the basic structure of Feature extractor
* TODO: wrap input and output to class
*/
class FeatureExtractor {
public: 
    /**
    * add this object to the static registry
    */
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
    virtual void initialize(t_systemConfigs sys_cfg) = 0;
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
    virtual t_featureValue send() = 0;
    /**
    * Check if this feature extractor is initialized
    */
    bool is_initialized(){return initialized;}
    void set_initialize_statue(bool s){initialized=s;}

protected:
    t_samples       x; //input
    t_featureValue  y; //output g
    t_systemConfigs system_configs;
    bool initialized=false; // 
}; // Class FeatureExtractor

}  // Namespace zerr 
#endif // FEATUREEXTRACTOR_H





