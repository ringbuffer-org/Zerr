/**
 * @file utils.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief  Utilities of Zerr* system - Contains common utility functions used throughout the library
 * @date 2024-02-13
 *
 * @copyright Copyright (c) 2023-2024
 */
#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdarg>
#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "types.h"

namespace zerr {
/**
 * @brief Format string in the style of printf
 * @param format String template to be formatted
 * @param ... Variable arguments to be inserted into the format string
 * @return std::string The formatted string with arguments inserted
 */
std::string formatString(const char* format, ...);
/**
 * @brief Convert a vector to a string with tab separation between elements
 * @param vector Vector of elements to be formatted
 * @return std::string Space-separated string representation of the vector elements
 */
// template<typename T>
// std::string formatVector(std::vector<T> vector);
template <typename T> std::string formatVector(const std::vector<T>& vector)
{
    std::string formated = "";
    for (size_t i = 0; i < vector.size(); ++i) {
        formated = formated + "  " + std::to_string(vector[i]);
    }
    return formated;
}
/**
 * @brief Compare a floating point number with 1 within a specified epsilon
 * @param value The floating point number to compare with 1
 * @param epsilon Maximum allowed difference from 1 (defaults to machine epsilon)
 * @return bool True if the value is within epsilon of 1, false otherwise
 */
bool isEqualTo1(Param value, Param epsilon = std::numeric_limits<Param>::epsilon());
/**
 * @brief Compare a floating point number with 0 within a specified epsilon
 * @param value The floating point number to compare with 0
 * @param epsilon Maximum allowed difference from 0 (defaults to machine epsilon)
 * @return bool True if the value is within epsilon of 0, false otherwise
 */
bool isEqualTo0(Param value, Param epsilon = std::numeric_limits<Param>::epsilon());
/**
 * @brief Calculate a sample value of a Hanning window function
 * @param pos Position within the window (0 to L-1)
 * @param L Total length of the Hanning window
 * @return float The Hanning window coefficient at the specified position
 */
inline float get_hann_sample(int pos, int L)
{
    float val = 0.5 * (1.0 - cos((2.0 * pi * static_cast<float>(pos)) / static_cast<float>(L)));
    return val;
}
/**
 * @brief Check if an element exists in a vector
 * @param element The element to search for
 * @param vector The vector to search within
 * @return bool True if the element is found in the vector, false otherwise
 */
template <typename T> bool isInVec(T element, const std::vector<T>& vector);
/**
 * @brief Check if a key exists in a map
 * @param element The key to search for
 * @param map The map to search within
 * @return bool True if the key exists in the map, false otherwise
 */
template <typename T, typename U> bool isInKey(T element, const std::map<T, U>& map)
{
    auto it = map.find(element);
    return it == map.end() ? false : true;
}
/**
 * @brief Apply a moving average filter to a sample buffer
 * @param segment Input sample buffer to be filtered
 * @param windowSize Size of the moving average window
 * @return Samples Filtered output sample buffer
 */
Samples applyMovingAverage(const Samples& segment, int windowSize);

} // namespace zerr
#endif // UTILS_H
