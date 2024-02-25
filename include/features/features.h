/**
 * @file features.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Include all feature head files 
 * @date 2023-07-27
 * 
 * @copyright Copyright (c) 2023
 */

#ifndef FEATURES_H
#define FEATURES_H

// time-domain features
#include "rootmeansquare.h"
#include "zerocrossingrate.h"
#include "crestfactor.h"

// spectral-domain features
#include "flux.h"
#include "centroid.h"
#include "rolloff.h"
#include "flatness.h"

// sample-level features
#include "zerocrossings.h"

#endif // FEATURES_H