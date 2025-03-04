#ifndef CENTROID_H
#define CENTROID_H

#include "configs.h"
#include "featureextractor.h"
#include "linearinterpolator.h"
#include "utils.h"

namespace zerr {
namespace feature {
/**
 * Spectral Centroid algorithm
 */
class Centroid : public FeatureExtractor {
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

    double freq_max;

    LinearInterpolator linear_interpolator;
};

}  // namespace feature
}  // namespace zerr
#endif  // CENTROID_H