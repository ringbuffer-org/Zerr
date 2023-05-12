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

// std::vector<double> randomVector(int size, double min, double max) {
//     std::vector<double> result(size);

//     srand(time(NULL)); // seed the random number generator

//     for (int i = 0; i < size; i++) {
//         double random = (double) rand() / RAND_MAX; // generate random number between 0 and 1
//         result[i] = min + random * (max - min); // scale the number to the desired range
//     }

//     return result;
// }



int main(int argc, char const *argv[]){
    print_unit_test_info();
    FeatureBank bank;

    typedef std::unique_ptr<FeatureExtractor> fe_ptr;

    bank.setup("Centroid");
    bank.shutdown("ZeroCrossing");
    bank.print_info("Onset");

    // Register the classes with the bank
    // bank.regist("Centroid", []() -> fe_ptr {
    //     return std::make_unique<Centroid>();
    // });
    bank.regist("Centroid", []() {
        return fe_ptr(new Centroid());
    });

    // bank.regist("ZeroCrossing", []() -> fe_ptr {
    //     return std::make_unique<ZeroCrossing>();
    // });

    bank.regist("ZeroCrossing", []() {
        return fe_ptr(new ZeroCrossing());
    });

    std::string className1 = "Centroid";
    fe_ptr object1 = bank.create(className1);
    if (object1) {
        std::cout<<"feature name:   "<<object1->get_name()<<std::endl;
    }

    std::string className2 = "ZeroCrossing";
    fe_ptr object2 = bank.create(className2);
    if (object2) {
        std::cout<<"feature name:   "<<object2->get_name()<<std::endl;
    }

    return 0;
}
