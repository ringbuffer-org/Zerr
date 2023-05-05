#ifndef CENTROID_H
#define CENTROID_H

#include "featureextractor.h"

namespace zerr {
/**
* Spectrum Centroid algorithm
*/
class Centroid : public FeatureExtractor {
public:
    Centroid(std::string name, std::string description, std::string category);
    void initialize();

private:
//     std::vector <double> x;
//     std::vector <double> y;
};

} //namespace zerr
#endif // CENTROID_H