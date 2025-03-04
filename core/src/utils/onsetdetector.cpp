/**
 * @file onsetdetector.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief onset detector util
 * @date 2024-02-16
 *
 * @copyright Copyright (c) 2023-2024
 */
#include "onsetdetector.h"

using zerr::Block;
using zerr::Index;
using zerr::OnsetDetector;

void OnsetDetector::setDebounceThreshold(int newThreshold) {
    debounceThreshold = newThreshold;
    lastOnsetPosition =
        -debounceThreshold;  // Reset lastOnsetPosition based on new threshold
}

void OnsetDetector::detectOnsetInBlock(Block& block) {
    if (block.empty()) return;

    for (size_t cnt = 0; cnt < block.size(); ++cnt) {
        if (block[cnt] == 1) {
            if (cnt - lastOnsetPosition >= debounceThreshold) {
                lastOnsetPosition = cnt;
            } else {
                block[cnt] = 0;
            }
        }
    }

    lastOnsetPosition = lastOnsetPosition - static_cast<int>(block.size());
}