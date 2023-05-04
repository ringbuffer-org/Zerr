#ifndef CENTROID_H
#define CENTROID_H

#include "featureextractor.h"

namespace zerr {
/**
* Spectrum Centroid algorithm
*/
class Centroid : public FeatureExtractor {
public:
    // Centroid();
    std::string get_name();
private:
    std::string name="centroid";
//     std::vector <double> x;
//     std::vector <double> y;
};

} //namespace zerr
#endif // CENTROID_H