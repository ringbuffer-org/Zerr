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
// std::vector<t_sample>& operator+=(std::vector<t_sample>& lhs, const std::vector<t_sample>& rhs) {
//     if (lhs.size() != rhs.size()) {
//         throw std::runtime_error("Vector sizes must be equal for element-wise addition.");
//     }

//     for (size_t i = 0; i < lhs.size(); ++i) {
//         lhs[i] += rhs[i];
//     }

//     return lhs;
// }

} // zerr
#endif //UTILS_H