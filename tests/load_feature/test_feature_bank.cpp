#include <iostream>
#include "featurebank.h"

using namespace zerr;

/**
* Unit test: Inspect algorithm folder and initialize general infos
* Usage: ./run_test 
*/
// void print_arguments(int argc, char const *argv[]){
//     std::cout <<"Number of arguments: "<< argc << std::endl;
//     for (int i = 0; i < argc; ++i){
//         std::cout << i << ": "<<argv[i] << std::endl;
//     }
// }

void print_unit_test_info(){
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Start testing feature bank class" << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

int main(int argc, char const *argv[]){
    print_unit_test_info();
    std::string path = "/Users/yangzeyu/Downloads/Zerr/features";
    FeatureBank fb(path);
    return 0;
}