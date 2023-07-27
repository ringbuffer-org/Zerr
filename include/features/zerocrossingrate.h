#ifndef ZEROCROSSINGRATE_H
#define ZEROCROSSINGRATE_H

#include "utils.h"
#include "configs.h"
#include "featureextractor.h"
#include "linearinterpolator.h"

namespace zerr {
namespace feature{
/**
* Zero Crossing Rate algorithm
*/
class ZeroCrossingRate : public FeatureExtractor { 
public:
    static const std::string name;        /**< Feature name: Zero Crossing Rate */
    static const std::string category;    /**< Feature category */
    static const std::string description; /**< Feature description */

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
    // params
    // int zero_crossings;

    LinearInterpolator linear_interpolator;
};

} //namespace feature
} //namespace zerr
#endif // ZEROCROSSINGRATE_H