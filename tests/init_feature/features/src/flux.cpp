#include "flux.h"
#include "zerr.h"
using namespace zerr;

Flux::Flux(std::string name, std::string description, std::string category): FeatureExtractor(name) {
    
}

void Flux::initialize(){
    std::cout<<"Flux:initialize"<<std::endl;
}
