#ifndef UTILS_H
#define UTILS_H

// standard libaries
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <unistd.h>
#include <limits>
#include <random>
#include <memory>
#include <chrono>
#include <thread>
#include <string.h>
#include <complex.h>

#include "types.h"

// dependencies
#include "yaml-cpp/yaml.h"
#include <fftw3.h>

namespace zerr{
/**
* Print the information of unit test
*/
void print_info(std::string info);

/**
* Print the topic of this unit test
*/
void print_line(int len);

/**
* Print a matrix of any type
* TODO: cannot use template for some reason
*/
// template <typename T>
void print_mat(std::vector<std::vector<float>> mat);

/**
* Print a vector of any type
*/
template <typename T>
void print_vec(std::vector<T> vec);

/**
* Generate a random vector with choosen length and minimum\maximum value
*/
// template <typename T>
std::vector<double> randomVector(int size, double min, double max);

/**
* get part of std::vector by index
*/
template<typename T>
std::vector<T> slice(std::vector<T> const &v, int m, int n)
{
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n + 1;
 
    std::vector<T> vec(first, last);
    return vec;
}

/**
* generate fake audio frames for testing
*/
// audio_mat gen_test_frames(int size, int num);
// inline float gaussian_lobe(int pos, double mu, double sigma, int L)
// {
//     double w     = exp( -(0.5) * pow( ((float) pos - mu) /sigma,2.0) );
//     return w;
// }


inline float get_hann_sample(int pos, int L)
{
    float val = 0.5 * (1.0 - cos( (2.0*PI* (float) pos) / (float)L) );
    return val;
}

} //namespace zerr
#endif //UTILS_H