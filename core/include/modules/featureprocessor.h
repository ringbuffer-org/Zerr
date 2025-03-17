#ifndef FEATUREPROCESSOR_H
#define FEATUREPROCESSOR_H

#include "featurebank.h"
#include "featureextractor.h"
#include "utils.h"

namespace zerr
{
    /**
     * This class merges outputs of feature extractors to generate control signals
     * This class is set to bypass for now
     * Cause when testing in PD, this could achieve easily by PD objects
     */
    class FeatureProcessor
    {
    public:
        /**
         * @brief initialize the Feature Processor
         *
         * @param N Number of feature extractors
         * @param mode Mode for processing feature values
         */
        void initialize(int N, std::string mode = "bypass");
        /**
         * @brief Update the input feature values buffer
         * @param in List of feature values from feature extractors
         */
        void fetch(t_featureValueList in);
        /**
         * @brief Apply the transformation to merge incoming feature values into new feature values
         * @details If in bypass mode, input feature values are copied directly to outputs
         */
        void process();
        /**
         * @brief Retrieve the processed feature values
         * @return List of processed feature values
         */
        t_featureValueList send();
        /**
         * @brief Reset the feature processor to initial state
         */
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

} // namespace zerr
#endif // FEATUREPROCESSOR