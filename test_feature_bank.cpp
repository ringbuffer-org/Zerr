#include <iostream>
#include <memory>

// //test data generator dependencies
#include <cstdlib>
#include <ctime>

#include "features.h"
#include "featureextractor.h"

using namespace zerr;
using namespace feature;

void print_unit_test_info(){
    std::cout << "--------------------------------" << std::endl;
    std::cout << "   Start testing feature bank   " << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

std::vector<double> randomVector(int size, double min, double max) {
    std::vector<double> result(size);

    srand(time(NULL)); // seed the random number generator

    for (int i = 0; i < size; i++) {
        double random = (double) rand() / RAND_MAX; // generate random number between 0 and 1
        result[i] = min + random * (max - min); // scale the number to the desired range
    }

    return result;
}



int main(int argc, char const *argv[]){
    print_unit_test_info();
    typedef std::unique_ptr<FeatureExtractor> Ptr;

    std::vector<Ptr> vec;
    vec.push_back(Ptr(new Centroid));
    vec.push_back(Ptr(new Onset));
    vec.push_back(Ptr(new ZeroCrossing));


    std::vector<double> test_frame = randomVector(1024, -1.0, 1.0);
    // std::vector<double> test_samples = randomVector(1, -1.0, 1.0);

    for (const auto& f : vec){
        std::cout<<"feature name:   "<<f->get_name()<<std::endl;
        std::cout<<"is_initialized: "<<f->is_initialized()<<std::endl;
        f->initialize();
        std::cout<<"is_initialized: "<<f->is_initialized()<<std::endl;
        std::cout<<"---------------------------------"<<std::endl;
    }
    // test centroid
    vec[0]->fetch(test_frame);
    vec[0]->extract();
    float res = vec[0]->send();
    std::cout<<"Result: "<<res<<std::endl;
    // test zero crossing
    std::vector<double> test_samples= {0.1, 2, -1.0, -2.0, 0.3};
    std::vector<double> tmp;
    for (const auto& s : test_samples){   
        tmp.clear();
        tmp.push_back(s);
        vec[2]->fetch(tmp);
        vec[2]->extract();
        res = vec[2]->send();
        std::cout<<"Result: "<<res<<std::endl;
    }

    return 0;
}
