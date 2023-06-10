#include "zerr.h"
#include "audiorouter.h"
#include "featurebank.h"
#include "mapper.h"
#include "trajectorygenerator.h"

using namespace zerr;

void print_unit_test_info(){
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Start connecting modules       " << std::endl;
    std::cout << "-------------------------------" << std::endl;
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

void print_vec(std::vector<float> vec){
    for (double sample:vec){
        std::cout<<sample<<" ";
    }
    std::cout<<std::endl;
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

template<typename T>
std::vector<T> slice(std::vector<T> const &v, int m, int n)
{
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n + 1;
 
    std::vector<T> vec(first, last);
    return vec;
}

int main(int argc, char const *argv[]){
    print_unit_test_info();
    typedef std::vector<std::vector<double>> audio_mat;
    typedef std::vector<std::string> str_vec;

    // test data
    int frame_size = 4; // 1024 4
    int num_frame = 8;  // 16 8
    audio_mat test_audio;
    std::vector<double> tmp = randomVector(frame_size*num_frame, -1.0, 1.0);
    for (int i = 0; i < num_frame; ++i){
        std::vector<double> sub = slice(tmp, i*frame_size, (i+1)*frame_size-1);
        test_audio.push_back(sub);
    }
    std::cout<<"Test audio size: "<<test_audio.size()<<"*"<<test_audio[0].size()<<std::endl;
    print_mat(test_audio);
    print_splite_line();

    /**
    *  start setup
    */
    // Featurebank setup
    FeatureBank bank;
    bank.regist_all();
    str_vec feature_names = {"Centroid", "ZeroCrossing"};  // 
    for (auto name : feature_names) {
        bank.setup(name);
    }
    bank.print_all_features();
    bank.print_active_features();
    bank.initialize();

    // TrajectoryGenerator setup
    TrajectoryGenerator gen;
    gen.initialize();

    // Mapper setup
    std::string config_path = "/Users/yangzeyu/Downloads/Zerr/configs/speakerarray/circulation_8.yaml"; //circulation_8
    Mapper mapper;
    mapper.initialize(config_path);

    AudioRouter router;
    router.initialize(frame_size, mapper.get_n_speaker() + 1); 

    /**
    *  start processing
    */
    print_splite_line();
    for (auto frame: test_audio){
        bank.fetch(frame);
        bank.process();

        gen.fetch(bank.send());
        gen.process();

        mapper.fetch(gen.send());
        mapper.process();

        router.fetch(frame, mapper.send());

        router.process();
        print_mat(router.send());
    }

    return 0;
}