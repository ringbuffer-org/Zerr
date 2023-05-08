#include "featureextractor.h"

using namespace zerr;

FeatureInfo::FeatureInfo(std::string n, std::string c, std::string d): name(n), category(c), description(d){}


FeatureExtractor::FeatureExtractor(){
    // registry_.push_back(this);
    // name = "centroid";
}