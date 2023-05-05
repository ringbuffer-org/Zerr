#ifndef FLUX_H
#define FLUX_H

#include "featureextractor.h"

namespace zerr {
/**
* Spectrum Centroid algorithm
*/
class Flux : public FeatureExtractor {
public:
    // Centroid();
    std::string get_name();
private:
    std::string name="flux";
//     std::vector <double> x;
//     std::vector <double> y;
};

} //namespace zerr
#endif // FLUX_H