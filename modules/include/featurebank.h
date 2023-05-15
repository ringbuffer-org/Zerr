#ifndef FEATUREBANK_H
#define FEATUREBANK_H

#include "zerr.h"
#include "features.h"
#include "featureextractor.h"

namespace zerr {

// typedef std::unique_ptr<FeatureExtractor> fe_ptr;
/**
* Organize and setup feature extractors
*/
class FeatureBank {
public:   
    typedef std::unique_ptr<FeatureExtractor> fe_ptr;
    typedef std::vector<fe_ptr> fe_ptr_vec;

    typedef std::vector<std::string> str_vec;
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
    void regist(const std::string& className, CreateFunc createFunc) {
        registed_features[className] = createFunc;
    }

    void regist_all();

    std::unique_ptr<FeatureExtractor> create(const std::string& className) {
        auto it = registed_features.find(className);
        if (it != registed_features.end()) {
            return it->second();
        }
        return nullptr;
    }
    /**
    * setup the feature selected by name 
    */
    void setup(std::string name);
    /**
    * desconstructe the used feature
    */
    void shutdown(std::string name);
    /**
    * Print the detail info of selected feature
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

    void initialize();
    void fetch(std::vector<double> in);
    void process();
    std::vector<float> send();
private:
    std::map<std::string, CreateFunc> registed_features;
    std::vector<fe_ptr> activated_features;

    std::vector<double> x;
    std::vector<float> ys;

    int n_features;

};

} //namespace zerr
#endif // FEATUREBANK_H

