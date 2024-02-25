/**
 * @file rolloff.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Implementation of spectral rolloff algorithm
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 */

#ifndef ROLLOFF_H
#define ROLLOFF_H

#include "utils.h"
#include "configs.h"
#include "featureextractor.h"
#include "linearinterpolator.h"

namespace zerr {
namespace feature{
/**
* The spectral rolloff is a measure used in signal processing to determine the frequency 
* below which a specified percentage of the total spectral energy lies. It is often used 
* to distinguish between harmonic and non-harmonic content in an audio signal.
*/
class Rolloff : public FeatureExtractor { 
public:
    static const std::string name;
    static const std::string category;
    static const std::string description;

    std::string get_name(){return name;}
    std::string get_category(){return category;}
    std::string get_description(){return description;}

    void initialize(SystemConfigs sys_cfg);
    void extract();
    void reset();
    void fetch(AudioInputs in);
    FeatureVals send();
    // FeatureVals perform(AudioInputs x);

private:
    void _reset_param();

    FeatureVal prv_y;
    FeatureVal crr_y;

    double freq_max;
    double rolloffPercent=0.85; //todo: add function to set this

    LinearInterpolator linear_interpolator;
};

} //namespace feature
} //namespace zerr
#endif // ROLLOFF_H
