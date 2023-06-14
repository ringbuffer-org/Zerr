#include "utils.h"
using namespace zerr;

namespace zerr{

void print_info(std::string info){
    // int len = info.size();
    std::cout<<info<<std::endl;
}

void print_line(int len){
    for (int i = 0; i < len; ++i){
        std::cout<<"-";
    }
    std::cout<<std::endl;
}

void print_mat(std::vector<std::vector<float>> mat){

    for (std::vector<float> chnl:mat){
        for (float sample:chnl){
            std::cout<<sample<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

// template <typename T>
void print_vec(std::vector<float> vec){

    for (auto sample:vec){
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

// audio_mat gen_test_frames(int size, int num){
//     audio_mat test_audio;
//     std::vector<double> tmp = randomVector(size*num, -1.0, 1.0);
//     for (int i = 0; i < num; ++i){
//         std::vector<double> sub = slice(tmp, i*size, (i+1)*size-1);
//         test_audio.push_back(sub);
//     }
//     std::cout<<"Test audio size: "<<test_audio.size()<<"*"<<test_audio[0].size()<<std::endl;

//     return test_audio;
// }


} // namespace zerr






