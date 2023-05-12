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
    // FeatureBank();
    /**
    * Function pointer type for creating objects
    */
    using CreateFunc = std::unique_ptr<FeatureExtractor> (*)();
    /**
    * Register a feature extractor with the bank
    */
    /**
    * static function that register all features
    */
    static void regist_all();
    /**
    * call the constructor of a feature based on the feature name
    */
    std::unique_ptr<FeatureExtractor> create(const std::string& className) {
        auto it = classRegistry_.find(className);
        if (it != classRegistry_.end()) {
            return it->second();
        }
        return nullptr;
    }
    /**
    * setup the feature selected by name 
    */
    // template<typename T>
    void setup(std::string name);
    /**
    * desconstructe the used feature
    */
    // template<typename T>
    void shutdown(std::string name);
    /**
    * Print all info of the selected feature
    * Only basic info for inactive feature
    * detail info for active feature
    */
    // template<typename T>
    void print_info(std::string name);
    /**
    * Print the name of all supported features
    */
    // void print_all_features();
    /**
    * Print the name of active features
    */
    // void print_active_features();
private:
    //TODO: Should all this be static?
    static std::map<std::string, CreateFunc> classRegistry_;

    //TODO: move the typedef to type.h
    typedef std::unique_ptr<FeatureExtractor> fe_ptr;
    typedef std::vector<fe_ptr> fe_ptr_vec;
    fe_ptr_vec active_features;

    static void regist(const std::string& className, CreateFunc createFunc) {
        classRegistry_[className] = createFunc;
    }
};

} //namespace zerr
#endif // FEATUREBANK_H

