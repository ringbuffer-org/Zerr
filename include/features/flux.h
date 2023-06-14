#ifndef FLUX_H
#define FLUX_H

#include "featureextractor.h"

namespace zerr {
namespace feature{
/**
* Spectrum Centroid algorithm
*/
class Flux : public FeatureExtractor {
public:
    static const std::string name; 
    static const std::string category;
    static const std::string description; 

    std::string get_name(){return name;}
    std::string get_category(){return category;}
    std::string get_description(){return description;}

    void initialize();
    void extract();
    void reset();
    void fetch(t_featureInputs in);
    t_featureValue send();

private:
    //TODO wrap the input and output
    t_featureInputs x; //input
    t_featureValue y; //output g

    // params
    float spec_max      = 0;
    float dist          = 0;
};

} //namespace feature
} //namespace zerr
#endif // FLUX_H