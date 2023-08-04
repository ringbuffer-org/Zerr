#ifndef UTILS_H
#define UTILS_H

// standard libaries
// #include <stdlib.h>
// #include <vector>
// #include <algorithm>
// #include <math.h>
// #include <unistd.h>
// #include <limits>
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


} // zerr
#endif //UTILS_H