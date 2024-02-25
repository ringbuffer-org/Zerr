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

    void initialize(SystemConfigs sys_cfg);
    void extract();
    void reset();
    void fetch(AudioInputs in);
    FeatureVals send();
    // FeatureVals perform(AudioInputs x);

private:
    void _reset_param();

    FeatureVal prv_y;
    FeatureVal crr_y;

    LinearInterpolator linear_interpolator;
};

} //namespace feature
} //namespace zerr
#endif // FLATNESS_H
