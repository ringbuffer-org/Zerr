#include "centroid.h"
#include "zerr.h"
using namespace zerr;

Centroid::Centroid(std::string name, std::string description, std::string category): FeatureExtractor(name) {

}

void Centroid::initialize(){
    std::cout<<"Centroid:initialize"<<std::endl;
}

