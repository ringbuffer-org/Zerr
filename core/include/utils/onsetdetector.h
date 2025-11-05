/**
 * @file onsetdetector.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief A utility class for detecting onsets (sudden increases in amplitude) in audio signals
 * @date 2024-02-16
 *
 * @copyright Copyright (c) 2023-2024
 */
#ifndef ONSETDETECTOR_H
#define ONSETDETECTOR_H

#include <iostream>
#include <vector>

#include "types.h"

namespace zerr {

/**
 * @class OnsetDetector
 * @brief Detects onsets in audio signals by analyzing amplitude changes
 *
 * This class implements onset detection by comparing consecutive samples and identifying
 * sudden increases in amplitude that exceed a threshold. It includes debounce functionality
 * to prevent multiple triggers from a single onset event.
 */
class OnsetDetector {
  public:
    /**
     * @brief Constructor with optional debounce threshold parameter
     * @param debounceThreshold Minimum number of samples between detected onsets
     *
     * Initializes an onset detector with specified debounce threshold. The debounce
     * threshold prevents multiple detections from the same onset by requiring a minimum
     * number of samples between detected onsets.
     */
    OnsetDetector(int debounceThreshold = 0) // Default to 0 for no debounce effect
        : lastSample(0), lastOnsetPosition(-debounceThreshold), debounceThreshold(debounceThreshold)
    {
    }
    /**
     * @brief Setter for debounceThreshold
     * @param newThreshold New minimum number of samples between detected onsets
     *
     * Updates the debounce threshold value used to prevent multiple detections
     * from the same onset event.
     */
    void setDebounceThreshold(int newThreshold);
    /**
     * @brief Function to detect onsets in blocks of the signal
     * @param block Reference to the audio block to analyze for onsets
     *
     * Analyzes a block of audio samples to detect onset events by comparing
     * consecutive samples and identifying significant amplitude increases.
     */
    void detectOnsetInBlock(Block& block);

  private:
    int lastSample;        ///< Previous sample value for amplitude comparison
    int lastOnsetPosition; ///< Sample position of the last detected onset
    int debounceThreshold; ///< Minimum samples required between onset detections
};

} // namespace zerr
#endif // ONSETDETECTOR_H
