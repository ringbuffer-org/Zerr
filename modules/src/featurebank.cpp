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

        // std::cout<<"  -Category:    "<<acticated_features[i]->get_category()<<std::endl;
        // std::cout<<"  -Description: "<<acticated_features[i]->get_description()<<std::endl;
    }
}

// void FeatureBank::print_info(std::string name){

// }

void FeatureBank::regist_all(){
    regist("Centroid", []() {
        return fe_ptr(new Centroid());
    });
    regist("ZeroCrossing", []() {
        return fe_ptr(new ZeroCrossing());
    }); 
    // regist("Flux", []() {
    //     return fe_ptr(new Flux());
    // });
}