#include "featurebank.h"
using namespace zerr;
using namespace feature;

void FeatureBank::setup(std::string name){
    activated_features.push_back(create(name));
}

void FeatureBank::shutdown(std::string name){
    std::cout<<"shutdown:\n"<<name<<std::endl;
}

void FeatureBank::print_all_features(){
    std::cout<<"All registed features: "<<std::endl;
    for (const auto& pair : registed_features) {
        std::cout<<"  -Name: "<<pair.first<<std::endl;
    }
}

void FeatureBank::print_active_features(){
    std::cout<<"All activated features: "<<std::endl;
    for (int i = 0; i < activated_features.size(); ++i){
        std::cout<<"  -Name: "<<activated_features[i]->get_name()<<std::endl;
    }
}

void FeatureBank::initialize(){
    n_features = activated_features.size();
    for (int i = 0; i < n_features; ++i){
        activated_features[i]->initialize();
    }
    std::cout<<"activated_features.size(): "<<activated_features.size()<<std::endl;
    ys.resize(activated_features.size());
}

void FeatureBank::fetch(std::vector<double> in){
    x.clear();
    x = in;
}

void FeatureBank::process(){
    for (int i = 0; i < activated_features.size(); ++i){
        activated_features[i]->fetch(x);
        activated_features[i]->extract();
        ys[i] = activated_features[i]->send();
    }
}

std::vector<float> FeatureBank::send(){
    return ys;
}

void FeatureBank::regist_all(){
    regist("Centroid", []() {
        return fe_ptr(new Centroid());
    });
    regist("ZeroCrossingRate", []() {
        return fe_ptr(new ZeroCrossingRate());
    }); 
}