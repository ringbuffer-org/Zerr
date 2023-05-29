#ifndef ZEROCROSSING_H
#define ZEROCROSSING_H

#include "featureextractor.h"

namespace zerr {
namespace feature{
/**
* Zero Crossing rate algorithm
*/
class ZeroCrossingRate : public FeatureExtractor { 
public:

    static const std::string name; 
    static const std::string category;
    static const std::string description; 

    std::string get_name(){return name;}
    std::string get_category(){return category;}
    std::string get_description(){return description;}

    void initialize();
    void extract();
    void reset();
    void fetch(std::vector<double> in);
    float send();

private:
    std::vector<double> x; //input
    float    y; //output

    // params
    int zero_crossings;
};

} //namespace feature
} //namespace zerr
#endif // ZEROCROSSING_H