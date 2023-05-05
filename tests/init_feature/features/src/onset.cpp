#include "onset.h"
#include "zerr.h"
using namespace zerr;

Onset::Onset(std::string name, std::string description, std::string category): FeatureExtractor(name) {
    
}

void Onset::initialize(){
    std::cout<<"Onset:initialize"<<std::endl;
}
