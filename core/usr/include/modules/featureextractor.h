#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include "utils.h"

namespace zerr
{
    /**
     * @brief Virtual base class that defines the interface for audio feature extractors
     *
     * This class provides a common interface that all feature extraction algorithms must implement.
     * It handles initialization, processing, and data management for extracting audio features.
     */
    class FeatureExtractor
    {
    public:
        /**
         * @brief Virtual destructor for proper cleanup of derived classes
         */
        virtual ~FeatureExtractor() {};
        /**
         * @brief Get basic information about the feature extractor
         * @return Name of the feature extractor
         */
        virtual std::string get_name() = 0;
        /**
         * @brief Get the category this feature belongs to
         * @return Category name (e.g. "spectral", "temporal", etc.)
         */
        virtual std::string get_category() = 0;
        /**
         * @brief Get detailed description of what this feature measures
         * @return Description string
         */
        virtual std::string get_description() = 0;
        /**
         * @brief Initialize the feature extraction algorithm with system configuration
         * @param sys_cfg System configuration containing sample rate, block size etc.
         */
        virtual void initialize(SystemConfigs sys_cfg) = 0;
        /**
         * @brief Process input data and calculate feature values
         *
         * Runs the feature extraction algorithm on the data in the input buffer
         * and stores results in the output buffer
         */
        virtual void extract() = 0;
        /**
         * @brief Reset the feature extractor to initial state
         *
         * Clears buffers and resets any accumulated state
         */
        virtual void reset() = 0;
        /**
         * @brief Load new audio data into the input buffer
         * @param x Input audio data structure containing time/frequency domain signals
         */
        virtual void fetch(AudioInputs x) = 0;
        /**
         * @brief Retrieve the calculated feature values
         * @return Map of feature names to their computed values
         */
        virtual FeatureVals send() = 0;
        /**
         * @brief Check if the feature extractor is properly initialized
         * @return True if initialized, false otherwise
         */
        bool is_initialized() { return initialized; }
        /**
         * @brief Set the initialization status
         * @param s New initialization status
         */
        void set_initialize_statue(bool s) { initialized = s; }

    protected:
        Samples x;     /**< Input data buffer containing time or frequency domain samples */
        FeatureVals y; /**< Output buffer containing extracted feature values */

        SystemConfigs system_configs; /**< System configuration parameters */
        bool initialized = false;     /**< Tracks whether the extractor is initialized */
    }; // Class FeatureExtractor

} // Namespace zerr
#endif // FEATUREEXTRACTOR_H
