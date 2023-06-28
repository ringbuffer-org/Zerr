#ifndef CENTROID_H
#define CENTROID_H

#include "configs.h"
#include "featureextractor.h"

namespace zerr {
namespace feature{
/**
* Spectrum Centroid algorithm
*/
class Centroid : public FeatureExtractor { 
public:
    static const std::string name; 
    static const std::string category;
    static const std::string description; 

    std::string get_name(){return name;}
    std::string get_category(){return category;}
    std::string get_description(){return description;}

    void initialize(t_systemConfigs sys_cfg);
    void extract();
    void reset();
    void fetch(t_featureInputs in);
    t_featureValue send();

private:
    // params
    void _reset_param();

    double freq_max;
    double centroid = 0.0;
    double totalMagnitude = 0.0;
};

} //namespace feature
} //namespace zerr
#endif // CENTROID_H