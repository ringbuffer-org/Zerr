#include <iostream>
#include <memory>

#include "audiorouter.h"

using namespace zerr;

void print_unit_test_info(){
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Start testing audio routing       " << std::endl;
    std::cout << "----------------------------------" << std::endl;
}

void print_splite_line(){
    std::cout << "----------------------------------" << std::endl;
}

void print_mat(std::vector<std::vector<double>> mat){
    for (std::vector<double> chnl:mat){
        for (double sample:chnl){
            std::cout<<sample<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

int main(int argc, char const *argv[]){
    typedef std::vector<std::vector<double>> out_mat;
    print_unit_test_info();

    AudioRouter router;
    out_mat tmp;

    std::vector<double> test_audio_in1 = {0.32, 0.95, -0.93, -0.22};
    std::vector<float>  test_cntrl_in1 = {0.0, 0.0, 1.0, 0.5, 0.0, 0.0, 1.0, 0.0};
    router.initialize(4, 8);

    router.fetch(test_audio_in1, test_cntrl_in1);
    router.process();
    tmp = router.send();
    print_mat(tmp);

    print_splite_line();

    std::vector<double> test_audio_in2 = {0.84, 0.23, -0.99, -0.12};
    std::vector<float>  test_cntrl_in2 = {0.0, 1.0, 0.0, 0.2, 0.0, 0.0, 0.4, 0.0};

    router.fetch(test_audio_in2, test_cntrl_in2);
    router.process();
    tmp = router.send();
    print_mat(tmp);

    return 0;
}