#include "zerr.h"
using namespace zerr;

Zerr::Zerr(std::string zerrCfgFile, std::string spkrCfgFile){
    zerr_cfg = zerrCfgFile;
    spkr_cfg = spkrCfgFile;
}

void Zerr::initialize(){
    _initialize_audioclient();
    _initialize_zerr();
}

void Zerr::run(){
    // bank.fetch(frame);
    // bank.process();
    // gen.fetch(bank.send());
    // gen.process();

    // mapper.fetch(gen.send());
    // mapper.process();

    // router.fetch(frame, mapper.send());

    // router.process();
    // print_mat(router.send());

    _hold(); // run forever
}

void Zerr::_hold(){
    std::cout << "\nZerr is listening...\nPress Ctrl+C to stop..." << std::endl;
    
    auto startTime = std::chrono::steady_clock::now(); // Get the starting time

    while (true) {
        auto currentTime = std::chrono::steady_clock::now(); // Get the current time
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime); // Calculate elapsed time in seconds

        std::cout << "\rRunning time: " << elapsedTime.count() << " seconds"<< std::flush;

        // Delay for a short period to avoid excessive output
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Zerr::_initialize_zerr(){
    // try constexper
    bank.regist_all();

    // replace with inside one time setup function
    str_vec feature_names = {"Centroid", "ZeroCrossingRate"};
    for (auto name : feature_names) {
        bank.setup(name);
    }

    bank.print_all_features();
    bank.print_active_features();

    bank.initialize();
    gen.initialize();
    mapper.initialize(spkr_cfg);
    router.initialize(frame_size, mapper.get_n_speaker() + 1); 

}

void Zerr::_initialize_audioclient(){
    std::cout<<"_initialize_audioclient..."<<std::endl;
    
    
    
}



    // test data
    // int frame_size = 11; // 1024 4
    // int num_frame = 4;  // 16 8
    // audio_mat test_audio = gen_test_frames(frame_size, num_frame);
    // print_mat(test_audio);
    // print_line(32);


    // Featurebank setup
    // FeatureBank bank;
    // bank.regist_all();
    // str_vec feature_names = {"Centroid", "ZeroCrossingRate"};
    // for (auto name : feature_names) {
    //     bank.setup(name);
    // }
    // bank.print_all_features();
    // bank.print_active_features();
    // bank.initialize();

    // TrajectoryGenerator setup
    // TrajectoryGenerator gen;
    // gen.initialize();

    // Mapper setup
    // std::string config_path = "/Users/yangzeyu/Downloads/Zerr/configs/speakerarray/circulation_8.yaml"; //circulation_8
    // Mapper mapper;
    // mapper.initialize(config_path);

    // AudioRouter setup
    // AudioRouter router;
    // router.initialize(frame_size, mapper.get_n_speaker() + 1); 

    /**
    *  start processing
    */
    // print_line(64);
    // for (auto frame: test_audio){
    //     bank.fetch(frame);
    //     bank.process();
    //     gen.fetch(bank.send());
    //     gen.process();

    //     mapper.fetch(gen.send());
    //     mapper.process();

    //     router.fetch(frame, mapper.send());

    //     router.process();
    //     print_mat(router.send());
    // }

