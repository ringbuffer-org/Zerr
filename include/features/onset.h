#ifndef ONSET_H
#define ONSET_H

#include "featureextractor.h"

namespace zerr {
namespace feature{
/**
* Amplitude-based simple onset detection
*/
class Onset : public FeatureExtractor {
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
    void fetch(t_blockIn in);
    float send();

private:
    //TODO wrap the input and output
    t_blockIn x; //input
    float y; //output g
};

} //namespace feature
} //namespace zerr
#endif // ONSET_H