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
    std::cout << "Start testing feature bank      " << std::endl;
    std::cout << "--------------------------------" << std::endl;
}
void print_splite_line(){
    std::cout << "--------------------------------" << std::endl;
}

int main(int argc, char const *argv[]){
    print_unit_test_info();
    FeatureBank bank;
    bank.regist_all();

    typedef std::vector<std::string> str_vec;
    str_vec feature_names = {"Centroid", "ZeroCrossing"}; //

    for (auto name : feature_names) {
        // feature_vec.push_back(bank.create(name));
        bank.setup(name);
    }

    bank.print_all_features();
    print_splite_line();
    bank.print_active_features();

    return 0;
}

    // "make_unique" needs above C++11
    // Register the classes with the bank
    // bank.regist("Centroid", []() -> fe_ptr {
    //     return std::make_unique<Centroid>();
    // });
