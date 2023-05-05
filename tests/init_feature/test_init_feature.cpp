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

    std::string name1 = "Centroid";
    std::string description1 = "The spectral centroid is a measure used in digital signal processing to characterise a spectrum.";
    std::string category1 = "spectrum";

    std::string name2 = "Flux";
    std::string description2 = "The ure used in digital signal processing to characterise a spectrum.";
    std::string category2 = "specrum";

    std::string name3 = "Onset";
    std::string description3 = "The spectr measure used in digital signal processing to characterise a spectrum.";
    std::string category3 = "specm";

    vec.push_back(Ptr(new Centroid(name1,description1,category1)));
    vec.push_back(Ptr(new Flux(name2,description2,category2)));
    vec.push_back(Ptr(new Onset(name3,description3,category3)));

    for (const auto& f : vec){
        std::cout<<"name: "<<f->get_name()<<std::endl;
        f->initialize();
    }

    return 0;
}