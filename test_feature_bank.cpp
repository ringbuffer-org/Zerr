#include <iostream>
#include <memory>

// //test data generator dependencies
#include <cstdlib>
#include <ctime>

#include "features.h"
#include "featurebank.h"

using namespace zerr;
using namespace feature;

void print_unit_test_info(){
    std::cout << "--------------------------------" << std::endl;
    std::cout << "   Start testing feature bank   " << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

    // "make_unique" needs above C++11
    // Register the classes with the bank
    // bank.regist("Centroid", []() -> fe_ptr {
    //     return std::make_unique<Centroid>();
    // });

int main(int argc, char const *argv[]){
    print_unit_test_info();
    FeatureBank bank;

    typedef std::unique_ptr<FeatureExtractor> fe_ptr;
    typedef std::vector<std::string> str_vec;
    typedef std::vector<fe_ptr> fe_ptr_vec;

    bank.setup("Centroid");
    bank.shutdown("ZeroCrossing");
    bank.print_info("Onset");


    bank.regist("Centroid", []() {
        return fe_ptr(new Centroid());
    });
    bank.regist("ZeroCrossing", []() {
        return fe_ptr(new ZeroCrossing());
    });

    // std::string className1 = "Centroid";
    // fe_ptr object1 = bank.create(className1);
    // if (object1) {
    //     std::cout<<"feature name:   "<<object1->get_name()<<std::endl;
    // }

    // std::string className2 = "ZeroCrossing";
    // fe_ptr object2 = bank.create(className2);
    // if (object2) {
    //     std::cout<<"feature name:   "<<object2->get_name()<<std::endl;
    // }
    str_vec fe_names;
    fe_ptr_vec feature_vec;
    fe_names.push_back("Centroid");
    fe_names.push_back("ZeroCrossing");

    for (auto name : fe_names) {
        feature_vec.push_back(bank.create(name));
    }
    for (int i = 0; i < 2; ++i)
    {
        std::cout<<"feature name:   "<<feature_vec[i]->get_name()<<std::endl;
    }
    

    return 0;
}
