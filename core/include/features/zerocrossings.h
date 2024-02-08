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

    void initialize(SystemConfigs sys_cfg);
    void extract();
    void reset();
    void fetch(AudioInputs in);
    FeatureVals send();
    // FeatureVals perform(AudioInputs x);

private:
    void _reset_param();
    bool _isZeroCrossing(Sample first, Sample second);

    Sample last_sample;
};

} //namespace feature
} //namespace zerr
#endif // ZEROCROSSINGS_H
