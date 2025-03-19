/**
 * @file rootmeansquare.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Implementation of root-mean-square algorithm
 * @date 2023-05-28
 *
 * @copyright Copyright (c) 2023
 */

#ifndef ROOT_MEAN_SQUARE
#define ROOT_MEAN_SQUARE

#include "configs.h"
#include "featureextractor.h"
#include "linearinterpolator.h"
#include "utils.h"

namespace zerr
{
    namespace feature
    {
        /**
         * @brief Root Mean Square algorithm - Calculates the quadratic mean amplitude of a signal
         *
         * The RMS value of a set of values (or a continuous-time waveform) is
         * the square root of the arithmetic mean of the squares of the values,
         * or the square of the function that defines the continuous waveform.
         * RMS is commonly used as a measure of signal power or amplitude level,
         * providing a more meaningful average than simple arithmetic mean for
         * oscillating signals.
         *
         * https://en.wikipedia.org/wiki/Root_mean_square
         */
        class RootMeanSquare : public FeatureExtractor
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
             * @brief Initialize the RMS extractor with system configurations
             * @param sys_cfg System configuration parameters
             */
            void initialize(SystemConfigs sys_cfg);

            /**
             * @brief Extract the RMS value from the current audio frame
             */
            void extract();

            /**
             * @brief Reset the RMS extractor state
             */
            void reset();

            /**
             * @brief Load new audio input data for processing
             * @param in Audio input data
             */
            void fetch(AudioInputs in);

            /**
             * @brief Get the calculated RMS values
             * @return FeatureVals The extracted RMS values
             */
            FeatureVals send();
            // FeatureVals perform(AudioInputs x);

        private:
            /**
             * @brief Reset internal parameters to initial state
             */
            void _reset_param();

            FeatureVal prv_y; ///< Previous RMS value
            FeatureVal crr_y; ///< Current RMS value

            LinearInterpolator linear_interpolator; ///< Interpolator for smoothing RMS values
        };

    } // namespace feature
} // namespace zerr

#endif // ROOT_MEAN_SQUARE