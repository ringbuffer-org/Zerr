#ifndef FLATNESS_H
#define FLATNESS_H

#include "utils.h"
#include "configs.h"
#include "featureextractor.h"
#include "linearinterpolator.h"

namespace zerr {
namespace feature{
/**
* Spectral Flatness algorithm
*/
class Flatness : public FeatureExtractor { 
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
    t_featureBuf send();

private:
    void _reset_param();

    t_featureValue prv_y;
    t_featureValue crr_y;

    LinearInterpolator linear_interpolator;
};

} //namespace feature
} //namespace zerr
#endif // FLATNESS_H
