#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include "zerr.h"
/*
Virtual Class of feature algoirthms
*/

namespace zerr {
template <typename T>
class FeatureInfo{
public:
    std::string name;
    std::string category;
    std::string description;

    typedef T* (*Creator)();
    Creator create;
    }
};

// template <typename BaseAlgorithm>
// class ESSENTIA_API AlgorithmInfo {
//  public:
//   typedef BaseAlgorithm* (*AlgorithmCreator)();

//   AlgorithmCreator create;
//   std::string name;
//   std::string description;
//   std::string category;
// };

class FeatureExtractor {
public: 
    /**
    * add this object to the static registry
    */
    FeatureExtractor(std::string n);
    /**
    * Virtual desctructor for dealing with virtual methods
    */
    virtual ~FeatureExtractor(){};
    /**
    * static member function to get all the classes in the same namespace
    */
    // static std::vector<FeatureExtractor*> get_all() {
    //     return registry_;
    // }

    std::string get_name(){return name;} 
    std::string get_description(){return description;} 
    std::string get_category(){return category;}
    /**
    * processing_mode: SAMPLE, BLOCK, FRAME
    * output_mode: FLOATING_POINTS, CATEGORY_INDEX, TRIGGER_BANG
    * (Or use simple float to represent all modes)
    */
    // std::string get_feature_name(){return name;}
    // std::string get_processing_mode(){return processing_mode;}
    // std::string get_output_mode(){return output_mode;}

    // int get_input_channel(){return input_channel;}
    // int get_sample_rate(){return sample_rate;}
    // int get_frame_size(){return frame_size;}
    // int get_step_length(){return step_length;}

    /**
    * Virtual constructor functions that setup basic configs for feature extractor
    */
    // virtual FeatureExtractor(){};
    /**
    * initialize the feature extraction algorithm 
    */
    virtual void initialize() = 0;
    /**
    * Run algorithm on the signal in the input buffer and update the output buffer
    */
    virtual void extract() = 0;
    /**
    * fetch samples from audio stream and load to the input buffer
    * #Note: This should be take cared by audio client rather the extractor itself.
    */
    // virtual void fetch() = 0;
    // static std::vector<FeatureExtractor*> registry_;

private:

    const std::string name; 
    const std::string description; 
    const std::string category;

    bool mInitialized;

    // struct featureInfo {
    //     std::string name; 
    //     std::string description; 
    //     std::string category;
    // };

    // std::string name; 
    // std::string processing_mode;
    // std::string output_mode;

    // int input_channel;
    // int sample_rate; 
    // int frame_size; 
    // int step_length;

    /**
    * This function will be called internal in the init method
    * check if the feature algorithm config has conflict with the audio client config
    * e.g frame_size, sample_rate, input_channel etc.
    */
    // virtual void conflict_check() = 0;

    /**
    * size varied according to the type of algorithm: 
    * sample:1 --> block:64 --> frame:1024 --> concated frames:1024*2
    */
    // std::vector <double> x;
    // std::vector <double> y;
}; // Class FeatureExtractor

} // Namespace zerr 

#endif // FEATUREEXTRACTOR_H
