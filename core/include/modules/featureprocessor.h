#ifndef FEATUREPROCESSOR_H
#define FEATUREPROCESSOR_H

#include "utils.h"
#include "featurebank.h"
#include "featureextractor.h"

namespace zerr {
/**
* This class merges outputs of feature extractors to generate control 
* This class is set to bypass for now
* Cause when testing in PD, this could achieve easily by PD objects
*/
class FeatureProcessor {
public:
    /**
    * @brief initialize the Trajectory Generator
    * 
    * @param N ....
    * @param mode ....
    */
    void initialize(int N, std::string mode="bypass");
    /**
    * update the input control signal buffer
    */
    void fetch(t_featureValueList in);
    /**
    * apply the transformation to merge incoming control signals into new control signals
    * if in the bypass mode inputs are copied directly to outputs
    */
    void process();

    t_featureValueList send();

    void reset();

private:
    std::string output_mode; // merge, bypass
    int n_features;

    t_featureValueList x;

    t_featureValueList y;

    /**
    * build blocks of feature to trajectory transformation
    * @params chnl: apply this function on which channel
    */
    float zero_crossing(float x, float thres); //
    float local_extrema(float x, float thres);
    float inflection(std::vector<float> xs);
    float threshold(std::vector<float> weights);
}; 

} //namespace zerr
#endif // FEATUREPROCESSOR