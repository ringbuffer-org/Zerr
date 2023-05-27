#include "zerr.h"
using namespace zerr;

Zerr::Zerr(std::string zerrCfgFile, std::string spkrCfgFile){
    zerr_cfg = zerrCfgFile;
    spkr_cfg = spkrCfgFile;
}

void Zerr::initialize(){
    _initialize_zerr();
    _initialize_audioclient();
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

    jack_activate(this->client);
    jack_connect (client, "system:capture_1", jack_port_name(input_port[0]));

    std::string system_playback_name;
    for (int i = 0; i < nOutputs; ++i){
        system_playback_name = "system:playback_" + std::to_string(i+nInputs+1); //+nInputs to avoid feedback loop
        jack_connect (client, jack_port_name(output_port[i]), system_playback_name.c_str());
    }

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
    client = jack_client_open("zerr", JackNullOption, &status, NULL);


    jack_set_process_callback(this->client, this->callback_process, this);

    // allocate jack input ports
    input_port = new jack_port_t*[nInputs];
    for (int i=0; i<nInputs; i++){
        std::string tmp_str = "input_" + std::to_string(i+1);
        input_port[i] = jack_port_register (client, tmp_str.c_str(),
        JACK_DEFAULT_AUDIO_TYPE,
        JackPortIsInput, 0);
    }

    // allocate jack output ports
    output_port = new jack_port_t*[nOutputs];
    for (int i=0; i<nOutputs; i++){
        std::string tmp_str = "output_" + std::to_string(i+1);
        output_port[i] = jack_port_register (client, tmp_str.c_str(),
        JACK_DEFAULT_AUDIO_TYPE,
        JackPortIsOutput, 0);
    }

    out = new jack_default_audio_sample_t*[nOutputs];
    in  = new jack_default_audio_sample_t*[nInputs];

    // jack_activate(this->client);
    // jack_connect (client, "system:capture_1", jack_port_name(input_port[0]));

    // std::string system_playback_name;
    // for (int i = 0; i < nOutputs; ++i){
    //     system_playback_name = "system:playback_" + std::to_string(i+nInputs+2); //+nInputs to avoid feedback loop
    //     jack_connect (client, jack_port_name(output_port[i]), system_playback_name.c_str());
    // }
    
}

int Zerr::callback_process(jack_nframes_t x, void* object)
{
  return static_cast<Zerr*>(object)->process(x);
}

int Zerr::process(jack_nframes_t nframes){
    // std::cout<<"..."<<std::endl;

    // get input buffers
    for ( int i=0 ; i<nInputs; i++)
    in[i]  = (jack_default_audio_sample_t *)
    jack_port_get_buffer(this->input_port[i], jack_get_buffer_size(client));

    // get output buffers
    for ( int i=0 ; i<nOutputs; i++)
    out[i] = (jack_default_audio_sample_t *)
    jack_port_get_buffer(this->output_port[i], jack_get_buffer_size(client));

    // set output buffer "0.0" // don't do this when using Puredata! because the input and output shares the same memory
    for(int chanCNT=0; chanCNT<nOutputs; chanCNT++)
    {
        for(int sampCNT=0; sampCNT<nframes; sampCNT++)
        out[chanCNT][sampCNT] = 0.0;
    }

    // just copy the input to all connected outputs for testing:works
    // for(int chanCNT=0; chanCNT<nOutputs; chanCNT++)
    // {
    //     for(int sampCNT=0; sampCNT<nframes; sampCNT++)
    //     out[chanCNT][sampCNT] = in[0][sampCNT];
    // }
    // std::cout<<"nframes: "<<nframes<<std::endl;

    // for(int chanCNT=0; chanCNT<nInputs; chanCNT++){
    //     for(int sampCNT=0; sampCNT<nframes; sampCNT++){
    //         // std::cout<<in[chanCNT][sampCNT]<<std::endl;
    //     }
    // }
    // print_mat(input_buffer);
    std::vector<double> targetData(in[0], in[0] + 256);

    bank.fetch(targetData);
    bank.process();
    gen.fetch(bank.send());
    gen.process();

    mapper.fetch(gen.send());
    mapper.process();

    router.fetch(targetData, mapper.send());

    router.process();

    // output_buffer.clear();
    // output_buffer = router.send();
    // std::vector<double> targetData(in[0], in[0] + 256);
    output_buffer = router.send();

    for(int chanCNT=0; chanCNT<nOutputs; chanCNT++){
        for(int sampCNT=0; sampCNT<nframes; sampCNT++)
        out[chanCNT][sampCNT] = static_cast<jack_default_audio_sample_t>(output_buffer[chanCNT][sampCNT]);
    }

    return 0;
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

