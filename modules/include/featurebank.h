#ifndef FEATUREBANK_H
#define FEATUREBANK_H

#include "zerr.h"
#include "features.h"
#include "featureextractor.h"

namespace zerr {
/**
* Organize and setup feature extractors
*/
class FeatureBank {
public:   
    /**
    * Construction function
    */
    FeatureBank();
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
    static const std::map<std::string, FeatureInfo> bank; 

    typedef std::unique_ptr<FeatureExtractor> fptr;
    static std::vector<fptr> activated;
};

} //namespace zerr
#endif // FEATUREBANK_H

