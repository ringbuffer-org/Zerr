#ifndef ZEROCROSSING_H
#define ZEROCROSSING_H

#include "featureextractor.h"

namespace zerr {
namespace feature{
/**
* Spectrum Centroid algorithm
*/
class ZeroCrossing : public FeatureExtractor { 
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
    //TODO wrap the input and output type into uniform class
    std::vector<double> x; //input
    float    y; //output

    // params
    void _reset_param();
    double new_x;
    double pre_x;
    float sign1;
    float sign2;
    
};

} //namespace feature
} //namespace zerr
#endif // ZEROCROSSING_H