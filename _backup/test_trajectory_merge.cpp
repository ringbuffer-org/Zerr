#include <iostream>
#include <memory>

#include "trajectorygenerator.h"

using namespace zerr;
using namespace feature;

void print_unit_test_info(){
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Start testing trajectory generator" << std::endl;
    std::cout << "----------------------------------" << std::endl;
}

void print_splite_line(){
    std::cout << "----------------------------------" << std::endl;
}

int main(int argc, char const *argv[]){
    print_unit_test_info();
    TrajectoryGenerator gen;
    gen.initialize();

    print_splite_line();

    std::vector<float> test_vec1 = {0.9, 0.6};
    gen.fetch(test_vec1);
    gen.process();
    std::cout<<"result: "<<gen.send()<<std::endl;

    print_splite_line();

    std::vector<float> test_vec2 = {0.3, 0.6};
    gen.fetch(test_vec2);
    gen.process();
    std::cout<<"result: "<<gen.send()<<std::endl;

    return 0;
}