#ifndef CENTROID_H
#define CENTROID_H

#include "featureextractor.h"

namespace zerr {
namespace feature{
/**
* Spectrum Centroid algorithm
*/
class Centroid : public FeatureExtractor { 
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
    void fetch(std::vector<float> in);
    float send();

private:
    //TODO wrap the input and output type into uniform class
    std::vector <float> x; //input
    float y; //output g

    // params
    void _reset_param();
    double k     = 0;
    double denum = 0.0000;
    double num   = 0.00001; // use a tiny offset to avoid NaNs
};

} //namespace feature
} //namespace zerr
#endif // CENTROID_H