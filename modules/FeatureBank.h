/*
This is a collection of possible features:
Single   Source :

Mutual   Sources:
    (This is adised by ChatGPT)
    1. inter-channel level difference
    2. inter-channel phase difference
    3. inter-channel coherence
    4. cross-correlation
    5. mutual information
Multiple Sources:

*/

#ifndef FEATUREBANK_H
#define FEATUREBANK_H
namespace zerr {
/**
* a structure template to store basic information of algorithms
*/
struct feature {
    std::string name; 
    std::string processing_mode;
    std::string output_mode;

    int input_channel;
    int sample_rate; 
    int frame_size; 
    int step_length;
};

class featurebank {
public:
    // general information
    std::string algorithm_folder='../algorithm';

    // all supported features
    feature centroid;
    // feature flatness;
    // feature flux;
    feature zero_crossing;
    feature level_diff;

    /**
    * Print all parameters of input featur
    */
    void print_info(feature ft);
    /**
    * Print the name of all supported features
    */
    void print_features();
    /**
    * Check the algorithm folder if there exists the algorithm source file
    * Raise warning when feature in the bank not finded in the folder
    */
    int consistency_check();
}


} //namespace zerr

#endif // FEATUREBANK_H

