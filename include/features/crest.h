#ifndef CREST_H
#define CREST_H

#include "configs.h"
#include "featureextractor.h"

namespace zerr {
namespace feature{
/**
* Crest algorithm
*/
class Crest : public FeatureExtractor { 
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

    void _reset_param();
};

} //namespace feature
} //namespace zerr
#endif // CREST_H