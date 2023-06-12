#ifndef TRAJECTORYGENERATOR_H
#define TRAJECTORYGENERATOR_H

#include "utils.h"
#include "featurebank.h"
#include "featureextractor.h"

namespace zerr {
/**
* This class merges outputs of feature extractors to generate control 
*/
class TrajectoryGenerator {
public:
    /**
    * initialize the feature extraction algorithms 
    * set up the input vec size
    */
    void initialize();
    /**
    * update the input control signal buffer
    */
    void fetch(t_featureValueList in);
    /**
    * apply the transformation to merge incoming control signals into mono control signal
    */
    void process();
    t_value send();
    void reset();

private:
    // std::vector<unique_ptr<FeatureExtractor>> extractors;
    /**
    * vector of name the feature extraction algorithms' uesed to generate this control trajectory
    */
    // std::vector<std::string> names;
    /**
    * input control signal buffer
    */
    t_featureValueList x;
    t_value y;
    // std::string output_mode;
    /**
    * all features should run on the same processing mode for real-time processing consistance
    */
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