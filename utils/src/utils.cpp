#include "utils.h"
using namespace zerr;

namespace zerr{

void print_unit_test_info(std::string info){
    int len = info.size();
    print_line(len);
    std::cout<<info<<std::endl;
    print_line(len);
}

void print_line(int len){
    for (int i = 0; i < len; ++i){
        std::cout<<"-";
    }
    std::cout<<std::endl;
}

// template <typename T>
void print_mat(std::vector<std::vector<double>> mat){
    // static_assert(IsNumberType<T>::value, "Invalid data type");

    for (std::vector<double> chnl:mat){
        for (double sample:chnl){
            std::cout<<sample<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

// template <typename T>
void print_vec(std::vector<double> vec){
    // static_assert(IsNumberType<T>::value, "Invalid data type");

    for (double sample:vec){
        std::cout<<sample<<" ";
    }
    std::cout<<std::endl;
}

// template <typename T>
std::vector<double> randomVector(int size, double min, double max) {
    // static_assert(IsNumberType<T>::value, "Invalid data type");

    std::vector<double> result(size);

    srand(time(NULL)); // seed the random number generator

    for (int i = 0; i < size; i++) {
        double random = (double) rand() / RAND_MAX; // generate random number between 0 and 1
        result[i] = min + random * (max - min); // scale the number to the desired range
    }

    return result;
}

} // namespace zerr






