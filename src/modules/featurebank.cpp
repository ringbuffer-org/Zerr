#include "featurebank.h"
using namespace zerr;
using namespace feature;


FeatureBank::FeatureBank():input_buffer(1024){
    _regist_all();
}


void FeatureBank::setup(std::string name){
    activated_features.push_back(_create(name));
}


void FeatureBank::shutdown(std::string name){
    std::cout<<"shutdown:\n"<<name<<std::endl;
}


void FeatureBank::print_info(std::string name){
    std::cout<<"print_info:\n"<<name<<std::endl;
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


void FeatureBank::initialize(t_featureList feature_names){

    for (auto name : feature_names) {
        activated_features.push_back(_create(name));
    }

    n_features = activated_features.size();
    for (int i = 0; i < n_features; ++i){
        activated_features[i]->initialize();
    }
    y_buf.resize(activated_features.size());
}


void FeatureBank::fetch(t_blockIn in){
    input_buffer.enqueue(in);
}


void FeatureBank::process(){
    for (int i = 0; i < activated_features.size(); ++i){
        activated_features[i]->fetch(input_buffer.getBufferSamples());
        activated_features[i]->extract();
        y_buf[i] = activated_features[i]->send();
    }
}


std::vector<float> FeatureBank::send(){
    std::cout<<y_buf[0]<<"  "<<y_buf[1]<<"  "<<y_buf[2]<<std::endl;
    return y_buf;
}


void FeatureBank::_regist_all(){
    _regist("Centroid", []() {
        return fe_ptr(new Centroid());
    });
    _regist("ZeroCrossingRate", []() {
        return fe_ptr(new ZeroCrossingRate());
    }); 
}


void FeatureBank::_regist(const std::string& className, CreateFunc createFunc) {
        registed_features[className] = createFunc;
    }


std::unique_ptr<FeatureExtractor> FeatureBank::_create(const std::string& className) {
        auto it = registed_features.find(className);
        if (it != registed_features.end()) {
            return it->second();
        }
        return nullptr;
    }