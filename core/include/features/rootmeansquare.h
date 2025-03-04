/**
 * @file rootmeansquare.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Implementation of root-mean-square algorithm
 * @date 2023-05-28
 *
 * @copyright Copyright (c) 2023
 */

#ifndef ROOT_MEAN_SQUARE
#define ROOT_MEAN_SQUARE

#include "configs.h"
#include "featureextractor.h"
#include "linearinterpolator.h"
#include "utils.h"

namespace zerr {
namespace feature {
/**
 * The RMS value of a set of values (or a continuous-time waveform) is
 * the square root of the arithmetic mean of the squares of the values,
 * or the square of the function that defines the continuous waveform.
 *
 * https://en.wikipedia.org/wiki/Root_mean_square
 */
class RootMeanSquare : public FeatureExtractor {
  public:
    static const std::string name;
    static const std::string category;
    static const std::string description;

    std::string get_name() { return name; }
    std::string get_category() { return category; }
    std::string get_description() { return description; }

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

    LinearInterpolator linear_interpolator;
};

}  // namespace feature
}  // namespace zerr

#endif  // ROOT_MEAN_SQUARE