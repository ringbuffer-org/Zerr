#ifndef TRAJECTORYGENERATOR_H
#define TRAJECTORYGENERATOR_H

#include "utils.h"
#include "featurebank.h"
#include "featureextractor.h"

namespace zerr {
/**
* This class merges outputs of feature extractors to generate control 
* This class is set to bypass for now
* Cause when testing in PD, this could achieve easily by PD objects
*/
class TrajectoryGenerator {
public:
    /**
    * initialize the Trajectory Generator
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
    float above_threshold(float x, float thres); //
    float below_threshold(float x, float thres);
    float max(std::vector<float> xs);
    float min(std::vector<float> xs);
    float weight_add(std::vector<float> weights);
    float scale(float xmin, float xmax, float ymin, float ymax);
    float sample(float x, float impulse);
    float quantitative(float x, float minx, float step);
}; 

} //namespace zerr
#endif // TRAJECTORYGENERATOR_H