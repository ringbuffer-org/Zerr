#ifndef FLUX_H
#define FLUX_H

#include "featureextractor.h"

namespace zerr {
/**
* Spectrum Centroid algorithm
*/
class Flux : public FeatureExtractor {
public:
    Flux(std::string name, std::string description, std::string category);
    void initialize();
private:
//     std::vector <double> x;
//     std::vector <double> y;
};

} //namespace zerr
#endif // FLUX_H