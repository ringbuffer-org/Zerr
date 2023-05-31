#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include "utils.h"

namespace zerr {
/**
* Virtual Class of all feature extractors
* Use this to unifiy the feature extractor behaviour
* TODO: wrap input and output to class
*/
class FeatureExtractor {
    /**
    * processing_mode: SAMPLE, BLOCK, FRAME
    * output_mode: FLOATING_POINTS, CATEGORY_INDEX, TRIGGER_BANG
    */
public: 
    /**
    * add this object to the static registry
    */
    // FeatureExtractor();
    /**
    * Virtual desctructor for dealing with virtual methods
    */
    virtual ~FeatureExtractor(){};
    /**
    * get function to access the private members
    * basic info(name, category, description) can be accessed before 
    */
    virtual std::string get_name() = 0;
    virtual std::string get_category() = 0;
    virtual std::string get_description() = 0;
    /**
    * initialize the feature extraction algorithm 
    */
    virtual void initialize() = 0;
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
    * #Note: This should be take cared by audio client rather the extractor itself.
    */
    virtual void fetch(input_vec x) = 0;
    virtual float send() = 0;
    /**
    * Check if this feature extractor is initialized
    */
    bool is_initialized(){return initialized;}
    void set_initialize_statue(bool s){initialized=s;}

private:

    bool initialized=false; // 

    // int x_size;
    // int y_size;
    // std::vector<double> x;
    // std::vector<double> y;

}; // Class FeatureExtractor
} // Namespace zerr 

#endif // FEATUREEXTRACTOR_H

    // int input_channel;
    // int sample_rate; 
    // int frame_size; 
    // int step_length;


// template <typename T>
// class FeatureInfo{
// public:
//     FeatureInfo(std::string, std::string, std::string);
//     std::string name;
//     std::string category;
//     std::string description;
//     void print();
// };
    // int get_input_channel(){return input_channel;}
    // int get_sample_rate(){return sample_rate;}
    // int get_frame_size(){return frame_size;}
    // int get_step_length(){return step_length;}
    // std::string get_feature_name(){return name;}
    // std::string get_processing_mode(){return processing_mode;}
    // std::string get_output_mode(){return output_mode;}

// template<typename InputType>
// class BaseInput{
// public:
//     BaseInput();
//     /**
//     * get the size of input vector 
//     */
//     int get_length(){return length;}
//     /**
//     * initialize the feature extraction algorithm 
//     */
    
//     InputType fetch(); 
//     /**
//     * initialize the feature extraction algorithm 
//     */
//     void set(); 
// private:
//     int length;
//     InputType in; //constant size
// }




