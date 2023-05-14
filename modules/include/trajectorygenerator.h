#ifndef TRAJECTORYGENERATOR_H
#define TRAJECTORYGENERATOR_H

#include "zerr.h"
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
    */
    void initialize();
    /**
    * update the input control signal buffer
    */
    void update();
    /**
    * apply the transformation to merge incoming control signals into mono control signal
    */
    void merge();

private:
    // std::vector<unique_ptr<FeatureExtractor>> extractors;
    /**
    * vector of name the feature extraction algorithms' uesed to generate this control trajectory
    */
    // std::vector<std::string> names;
    /**
    * input control signal buffer
    */
    std::vector<float> values;
    std::string output_mode;
    /**
    * all features should run on the same processing mode for real-time processing consistance
    */
    // void check_processing_mode();
    /**
    * build blocks of feature to trajectory transformation
    * @params chnl: apply this function on which channel
    * /TODO: use self-defined float type(floating, category, trigger)?
    */
    float above_threshold(float x, float thres); //
    float below_threshold(float x, float thres);
    float max(std::vector<float> xs);
    float min(std::vector<float> xs);
    float weight_add(std::vector<float> xs, std::vector weights);
    float scale(float xmin, float xmax, float ymin, float ymax);
    float sample(float x, float impulse);
    float quantitative(float x, float minx, float step);
}; 

} //namespace zerr
#endif // TRAJECTORYGENERATOR_H