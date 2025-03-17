#ifndef CREST_H
#define CREST_H

#include "configs.h"
#include "featureextractor.h"
#include "linearinterpolator.h"
#include "utils.h"

namespace zerr
{
    namespace feature
    {
        /**
         * @brief Crest Factor algorithm - Calculates the ratio of peak amplitude to RMS value
         *
         * The crest factor, also known as peak-to-average ratio (PAR), measures the waveform's
         * dynamic range by comparing its peak values to the RMS level. Higher crest factors
         * indicate more peaky signals with greater dynamic range, while lower values suggest
         * more consistent amplitude levels.
         */
        class CrestFactor : public FeatureExtractor
        {
        public:
            static const std::string name;        ///< Name identifier for this feature
            static const std::string category;    ///< Category this feature belongs to
            static const std::string description; ///< Description of what this feature measures

            /**
             * @brief Get the name identifier of this feature
             * @return std::string The feature name
             */
            std::string get_name() { return name; }

            /**
             * @brief Get the category this feature belongs to
             * @return std::string The feature category
             */
            std::string get_category() { return category; }

            /**
             * @brief Get the description of what this feature measures
             * @return std::string The feature description
             */
            std::string get_description() { return description; }

            /**
             * @brief Initialize the crest factor extractor with system configurations
             * @param sys_cfg System configuration parameters
             */
            void initialize(SystemConfigs sys_cfg);

            /**
             * @brief Extract the crest factor from the current audio frame
             */
            void extract();

            /**
             * @brief Reset the crest factor extractor state
             */
            void reset();

            /**
             * @brief Load new audio input data for processing
             * @param in Audio input data
             */
            void fetch(AudioInputs in);

            /**
             * @brief Get the calculated crest factor values
             * @return FeatureVals The extracted crest factor values
             */
            FeatureVals send();
            // FeatureVals perform(AudioInputs x);

        private:
            /**
             * @brief Reset internal parameters to initial state
             */
            void _reset_param();

            FeatureVal prv_y; ///< Previous crest factor value
            FeatureVal crr_y; ///< Current crest factor value

            LinearInterpolator linear_interpolator; ///< Interpolator for smoothing crest factor values
        };

    } // namespace feature
} // namespace zerr
#endif // CREST_H
