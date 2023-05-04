#ifndef FEATUREBANK_H
#define FEATUREBANK_H

#include "zerr.h"
#include "featureextractor.h"

namespace zerr {
/*
The class to organize and setup feature extractors in zerr
*/
class FeatureBank {
public:   
    /**
    * a structure template to store basic information of features
    * TODO: move this to types.h file in the future
    */
    struct featureInfo {
        std::string name; 
        std::string description; 
        std::string category;
    };
    /**
    * Construction function: set algorithm path
    */
    FeatureBank(std::string path);
    /**
    * setup the feature selected by name 
    */
    void setup(std::string name);
    /**
    * desconstructe the used feature
    */
    void shutdown(std::string name);
    /**
    * Print all info of the selected feature
    * Only basic info for inactive feature
    * detail info for active feature
    */
    void print_info(std::string name);
    /**
    * Print the name of all supported features
    */
    void print_all_features();
    /**
    * Print the name of active features
    */
    void print_active_features();
private:
    // general information
    std::string path; // the relative path to the algorithm folder
    std::vector<featureInfo> all_features; 
    std::vector<FeatureExtractor> active_features;
};

} //namespace zerr
#endif // FEATUREBANK_H

