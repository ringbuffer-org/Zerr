#include <iostream>
#include <memory>

#include "mapper.h"

using namespace zerr;

void print_unit_test_info(){
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Start testing mapping strategy    " << std::endl;
    std::cout << "----------------------------------" << std::endl;
}

void print_splite_line(){
    std::cout << "----------------------------------" << std::endl;
}

int main(int argc, char const *argv[]){
    print_unit_test_info();

    std::string config_path = "/Users/yangzeyu/Downloads/Zerr/configs/speakerarray/circulation_8.yaml"; //circulation_8
    Mapper mapper;
    mapper.initialize(config_path);

    print_splite_line();

    std::vector<float> test_in = {0,0,1,1,0,1,0,0,1,0,1,0,1};
    for (float x : test_in){
        mapper.fetch(x);
        mapper.process();
        mapper.send();
    }

    return 0;
}