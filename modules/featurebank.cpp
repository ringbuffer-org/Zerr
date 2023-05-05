#include "featurebank.h"
using namespace zerr;

FeatureBank::FeatureBank(std::string path){
    features_path = path;
    std::cout<<"The Path of feature algorithms:\n"<<features_path<<std::endl;
    // initialize the static member variable of the base class





    // std::string name = "Spectrum Centroid";
    // create some objects of the classes
    Centroid obj1;
    // MyClass2 obj2;

    // get all the classes in the same namespace
    auto classes = FeatureExtractor::get_all();

    std::cout<<"Name: "<<obj1.get_name()<<std::endl;
    // std::cout<<"Description: "<<obj1.get_description()<<std::endl;
    // std::cout<<"Category: "<<obj1.get_category()<<std::endl;

    // print the names of the classes to the console
    // std::cout << "Classes in the same namespace: ";
    // for (const auto& cls : classes) {
    //     std::cout << cls->get_name() << " ";
    // }
    // std::cout << std::endl;



















}

void FeatureBank::setup(std::string name){
    std::cout<<"setup:\n"<<name<<std::endl;
}

void FeatureBank::shutdown(std::string name){
    std::cout<<"shutdown:\n"<<name<<std::endl;
}

void FeatureBank::print_info(std::string name){
    std::cout<<"shutdown:\n"<<name<<std::endl;
}

