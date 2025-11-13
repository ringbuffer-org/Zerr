/**
 * @file audio_features.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Header file that includes all audio feature extraction headers
 * @date 2023-07-27
 *
 * @copyright Copyright (c) 2023
 *
 * This header provides a centralized include point for all audio feature extraction
 * functionality. It groups the features into three main categories:
 * - Time-domain features: Statistical measures computed directly from audio samples
 * - Spectral-domain features: Measures computed from frequency-domain representations
 * - Sample-level features: Features computed on individual samples rather than blocks
 */

#ifndef FEATURES_H
#define FEATURES_H

// Time-domain features - Computed directly from audio samples
#include "crestfactor.h"      // Peak-to-RMS ratio
#include "rootmeansquare.h"   // Root mean square amplitude
#include "zerocrossingrate.h" // Rate of signal polarity changes

// Spectral-domain features - Computed from frequency-domain representation
#include "centroid.h" // Spectral center of mass
#include "flatness.h" // Geometric vs arithmetic mean ratio
#include "flux.h"     // Frame-to-frame spectral change
#include "rolloff.h"  // Frequency below which N% of spectrum lies

// Sample-level features - Computed per-sample rather than per-block
#include "zerocrossings.h" // Individual zero crossing points

#endif // FEATURES_H
