/**
 * @file onsetdetector.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief onset detector util
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

class OnsetDetector {
  public:
    /**
     * @brief Constructor with optional debounce threshold parameter
     */
    OnsetDetector(
        int debounceThreshold = 0)  // Default to 0 for no debounce effect
        : lastSample(0),
          lastOnsetPosition(-debounceThreshold),
          debounceThreshold(debounceThreshold) {}
    /**
     * @brief Setter for debounceThreshold
     */
    void setDebounceThreshold(int newThreshold);
    /**
     * @brief Function to detect onsets in blocks of the signal
     */
    void detectOnsetInBlock(Block& block);

  private:
    int lastSample;
    int lastOnsetPosition;
    int debounceThreshold;
};

}  // namespace zerr
#endif  // ONSETDETECTOR_H