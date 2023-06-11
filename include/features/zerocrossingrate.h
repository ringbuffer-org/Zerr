#ifndef ZEROCROSSINGRATE_H
#define ZEROCROSSINGRATE_H

#include "featureextractor.h"

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
    t_featureValue send();

private:
    t_audioBuf      x; //input
    t_featureValue  y; //output
    t_systemConfigs system_configs;

    // params
    int zero_crossings;
};

} //namespace feature
} //namespace zerr
#endif // ZEROCROSSINGRATE_H