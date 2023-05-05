#ifndef ONSET_H
#define ONSET_H

#include "featureextractor.h"

namespace zerr {
/**
* Spectrum Centroid algorithm
*/
class Onset : public FeatureExtractor {
public:
    Onset(std::string name, std::string description, std::string category);
    void initialize();
private:
//     std::vector <double> x;
//     std::vector <double> y;
};

} //namespace zerr
#endif // ONSET_H