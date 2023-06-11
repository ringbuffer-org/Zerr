#ifndef ONSET_H
#define ONSET_H

#include "featureextractor.h"

namespace zerr {
namespace feature{
/**
* Amplitude-based simple onset detection
*/
class Onset : public FeatureExtractor {
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
    t_audioBuf x; //input
    t_featureValue y; //output g
    t_systemConfigs system_configs;
};

} //namespace feature
} //namespace zerr
#endif // ONSET_H