#ifndef UTILS_H
#define UTILS_H

// standard libaries
// #include <stdlib.h>
// #include <vector>
// #include <algorithm>
#include <math.h>
// #include <unistd.h>
#include <limits>
// #include <random>
// #include <memory>
// #include <chrono>
// #include <thread>
#include <string>
// #include <complex.h>
#include <iostream>
#include <sstream>
#include <cstdarg>
#include <cstring>
#include "types.h"
#include <cassert>
#include <algorithm>

namespace zerr {
/**
* @brief format string in the style of printf
* @param format string to be formated
* @param ... the arguments for the string 
* @return std::string formated string
*/
std::string formatString(const char* format, ...);
/**
* @brief compare the float number with 1
* @param value the float number to be compared with 1
* @param epsilon the error threshold
* @return bool is equal to 1 or not
*/
bool isEqualTo1(t_value value, t_value epsilon = std::numeric_limits<t_value>::epsilon());
/**
* @brief compare the float number with 0
* @param value the float number to be compared with 0
* @param epsilon the error threshold
* @return bool is equal to 1 or not
*/
bool isEqualTo0(t_value value, t_value epsilon = std::numeric_limits<t_value>::epsilon());
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

} // zerr
#endif //UTILS_H
