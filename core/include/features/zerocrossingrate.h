#ifndef ZEROCROSSINGRATE_H
#define ZEROCROSSINGRATE_H

#include "configs.h"
#include "featureextractor.h"
#include "linearinterpolator.h"
#include "utils.h"

namespace zerr
{
    namespace feature
    {
        /**
         * @brief Zero Crossing Rate algorithm - Calculates how often a signal crosses the zero level
         *
         * The zero crossing rate measures the number of times an audio signal changes sign
         * (crosses zero amplitude) within a given frame, divided by the frame length. Higher
         * rates indicate more high frequency content or noise-like signals, while lower rates
         * suggest more low frequency or tonal content. This feature is useful for distinguishing
         * between voiced and unvoiced speech segments or detecting percussive sounds.
         */
        class ZeroCrossingRate : public FeatureExtractor
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
             * @brief Initialize the zero crossing rate extractor with system configurations
             * @param sys_cfg System configuration parameters
             */
            void initialize(SystemConfigs sys_cfg);

            /**
             * @brief Extract the zero crossing rate from the current audio frame
             */
            void extract();

            /**
             * @brief Reset the zero crossing rate extractor state
             */
            void reset();

            /**
             * @brief Load new audio input data for processing
             * @param in Audio input data
             */
            void fetch(AudioInputs in);

            /**
             * @brief Get the calculated zero crossing rate values
             * @return FeatureVals The extracted zero crossing rate values
             */
            FeatureVals send();
            // FeatureVals perform(AudioInputs x);

        private:
            /**
             * @brief Reset internal parameters to initial state
             */
            void _reset_param();

            FeatureVal prv_y; ///< Previous zero crossing rate value
            FeatureVal crr_y; ///< Current zero crossing rate value
            // params
            // int zero_crossings;

            LinearInterpolator linear_interpolator; ///< Interpolator for smoothing zero crossing rate values
        };

    } // namespace feature
} // namespace zerr
#endif // ZEROCROSSINGRATE_H