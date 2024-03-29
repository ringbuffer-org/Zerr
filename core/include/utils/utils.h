/**
 * @file utils.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief  Utilities of Zerr* system
 * @date 2024-02-13
 * 
 * @copyright Copyright (c) 2023-2024
 */
#ifndef UTILS_H
#define UTILS_H

// #include <iostream>
// #include <math.h>
// #include <limits>
// #include <string>
// #include <sstream>
// #include <cstdarg>
// #include <cstring>
// #include <cassert>
// #include <algorithm>
// #include <vector>

#include "types.h"

namespace zerr {
/**
* @brief format string in the style of printf
* @param format string to be formated
* @param ... the arguments for the string 
* @return std::string formated string
*/
std::string formatString(const char* format, ...);
/**
* @brief transfer vector to string with tab separated
* @param vector vector to be formated
* @return std::string formated string
*/
// template<typename T>
// std::string formatVector(std::vector<T> vector);
template<typename T>
std::string formatVector(std::vector<T> vector){
    std::string formated = "";
    for (size_t i = 0; i < vector.size(); ++i){
        formated = formated + "  " + std::to_string(vector[i]);
    }
    return formated;
}
/**
* @brief compare the float number with 1
* @param value the float number to be compared with 1
* @param epsilon the error threshold
* @return bool is equal to 1 or not
*/
bool isEqualTo1(Param value, Param epsilon = std::numeric_limits<Param>::epsilon());
/**
* @brief compare the float number with 0
* @param value the float number to be compared with 0
* @param epsilon the error threshold
* @return bool is equal to 1 or not
*/
bool isEqualTo0(Param value, Param epsilon = std::numeric_limits<Param>::epsilon());
/**
* @brief calculate the sample of hanning window on the fly
* @param pos the position in hanning window
* @param L the length of hanning window
* @return hanning window sample at given position and window length
*/
inline float get_hann_sample(int pos, int L){
    float val = 0.5 * (1.0 - cos( (2.0*PI* (float) pos) / (float)L) );
    return val;
}
/**
* @brief find if certain element is inside the std::vector
* @param element the element to be found
* @param vector the std:vector may contain the element
* @return bool is the element in the vector or not
*/
template<typename T>
bool isInVec(T element, std::vector<T> vector);
/**
* @brief find if certain element is inside the std::vector
* @param element the element to be found
* @param vector the std:vector may contain the element
* @return bool is the element in the vector or not
*/
template<typename T, typename U>
bool isInKey(T element, std::map<T, U> map){
    auto it = map.find(element);
    return it == map.end()?false:true;
}
/**
* @brief TODO
* @param TODO
* @param TODO
* @return TODO
*/
Samples applyMovingAverage(const Samples& segment, int windowSize);

} // zerr
#endif //UTILS_H
