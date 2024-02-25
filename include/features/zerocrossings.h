#ifndef ZEROCROSSINGS_H
#define ZEROCROSSINGS_H

#include "utils.h"
#include "configs.h"
#include "featureextractor.h"

namespace zerr {
namespace feature{
/**
* ZeroCrossings algorithm
*/
class ZeroCrossings : public FeatureExtractor { 
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
    bool _isZeroCrossing(t_sample first, t_sample second);

    t_sample last_sample;
};

} //namespace feature
} //namespace zerr
#endif // ZEROCROSSINGS_H