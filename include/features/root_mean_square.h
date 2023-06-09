#ifndef ROOT_MEAN_SQUARE  
#define ROOT_MEAN_SQUARE

#include "configs.h"
#include "featureextractor.h"

namespace zerr {
namespace feature{
/**
* Root-mean-square algorithm
*/
class RootMeanSquare : public FeatureExtractor { 
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
    int wave_size;

    void _reset_param();

    double square_sum;
    double square_root;
};

} //namespace feature
} //namespace zerr
#endif // ROOT_MEAN_SQUARE