/**
 * @file audio_features.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Include all feature head files
 * @date 2023-07-27
 *
 * @copyright Copyright (c) 2023
 */

#ifndef FEATURES_H
#define FEATURES_H

// time-domain features
#include "crestfactor.h"
#include "rootmeansquare.h"
#include "zerocrossingrate.h"

// spectral-domain features
#include "centroid.h"
#include "flatness.h"
#include "flux.h"
#include "rolloff.h"

// sample-level features
#include "zerocrossings.h"

#endif  // FEATURES_H