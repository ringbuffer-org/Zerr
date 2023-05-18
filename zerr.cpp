#include "zerr.h"
using namespace zerr;

int main(int argc, char const *argv[]){
    print_unit_test_info("main test");
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
    print_line(32);
    print_mat(test_audio);
    print_line(32);

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
    print_line(64);
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