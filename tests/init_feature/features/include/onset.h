#ifndef ONSET_H
#define ONSET_H

#include "featureextractor.h"

namespace zerr {
/**
* Spectrum Centroid algorithm
*/
class Onset : public FeatureExtractor {
public:
    // Centroid();
    std::string get_name();
private:
    std::string name="onset";
//     std::vector <double> x;
//     std::vector <double> y;
};

} //namespace zerr
#endif // ONSET_H