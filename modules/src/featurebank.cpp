#include "featurebank.h"
using namespace zerr;



void FeatureBank::setup(std::string name){
    active_features.push_back(create(name));
}

void FeatureBank::shutdown(std::string name){
    std::cout<<"shutdown:\n"<<name<<std::endl;
}

void FeatureBank::print_info(std::string name){
    std::cout<<"shutdown:\n"<<name<<std::endl;
}

static void FeatureBank::regist_all(){
    regist("Centroid", []() {
        return fe_ptr(new Centroid());
    });
    regist("ZeroCrossing", []() {
        return fe_ptr(new ZeroCrossing());
    });
}




