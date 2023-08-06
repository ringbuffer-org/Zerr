#ifndef UTILS_H
#define UTILS_H

// standard libaries
// #include <stdlib.h>
// #include <vector>
// #include <algorithm>
// #include <math.h>
// #include <unistd.h>
#include <limits>
// #include <random>
// #include <memory>
// #include <chrono>
// #include <thread>
// #include <string.h>
// #include <complex.h>
#include <iostream>
#include <sstream>
#include <cstdarg>

#include "types.h"

namespace zerr {
/**
* @brief EnvelopeGenerator generates evelope stream according to input control signal and the speaker array setups
*/
std::string formatString(const char* format, ...);
/**
* @brief TODO
*/
bool isEqualTo1(t_value value, t_value epsilon = std::numeric_limits<t_value>::epsilon());
/**
* @brief TODO
*/
bool isEqualTo0(t_value value, t_value epsilon = std::numeric_limits<t_value>::epsilon());
/**
* @brief TODO
*/
inline float get_hann_sample(int pos, int L){
    float val = 0.5 * (1.0 - cos( (2.0*PI* (float) pos) / (float)L) );
    return val;
}

} // zerr
#endif //UTILS_H