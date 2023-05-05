#include <iostream>
#include <memory>
#include "features.h"
#include "featureextractor.h"
using namespace zerr;

// std::vector<FeatureExtractor*> FeatureExtractor::registry_;

void print_unit_test_info(){
    std::cout << "---------------------------------" << std::endl;
    std::cout << " Start testing feature extractor " << std::endl;
    std::cout << "---------------------------------" << std::endl;
}

int main(int argc, char const *argv[]){
    print_unit_test_info();
    typedef std::unique_ptr<FeatureExtractor> Ptr;

    std::vector<Ptr> vec;
    
    std::string n = "centroid123";
    std::string d = "The spectral centroid is a measure used in digital signal processing to characterise a spectrum.";
    std::string c = "spectrum";

    vec.push_back(Ptr(new Centroid(n,d,c)));
    // vec.push_back(Ptr(new Flux()));
    // vec.push_back(Ptr(new Onset()));

    for (const auto& f : vec){
        std::cout<<"name: "<<f->get_name()<<std::endl;
        f->initialize();
    }

    // Centroid c1;
    // std::cout<<"Centroid name: "<<c1.get_name()<<std::endl;
    // Flux f1;
    // std::cout<<"Flux name: "<<f1.get_name()<<std::endl;
    // Onset o1;
    // std::cout<<"Onset name: "<<o1.get_name()<<std::endl;

    // get all the classes in the same namespace
    // auto classes = FeatureExtractor::get_all();

    // print the names of the classes to the console
    // std::cout << cls->get_name() << "\n";
    // for (const auto& cls : classes) {
    //     std::cout << cls->get_name() << "\n";
    // }
    // std::cout << std::endl;

    return 0;
}