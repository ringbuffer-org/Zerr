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
    std::cout<<"initialize zerr..."<<std::endl;

    bank.initialize(feature_names, sys_cfg);

    #ifdef TESTMODE
    bank.print_all_features();
    bank.print_active_features();
    #endif // TESTMODE

    // gen.initialize();
    // mapper.initialize(spkr_cfg);
    // router.initialize(frame_size, mapper.get_n_speaker() + 1); 
}

void Zerr::_initialize_audioclient(){
    std::cout<<"initialize audioclient..."<<std::endl;
    client = jack_client_open("zerr", JackNullOption, &status, NULL);

    #ifdef TESTMODE
    sys_cfg.sample_rate = static_cast<size_t>(jack_get_sample_rate(client));
    sys_cfg.block_size  = static_cast<size_t>(jack_get_buffer_size(client));
    std::cout<<"sys_cfg.sample_rate -- "<<sys_cfg.sample_rate<<std::endl;
    std::cout<<"sys_cfg.block_size  -- "<<sys_cfg.block_size <<std::endl;
    #endif // TESTMODE

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

}

int Zerr::callback_process(jack_nframes_t x, void* object)
{
  return static_cast<Zerr*>(object)->process(x);
}

int Zerr::process(jack_nframes_t nframes){
    // get input buffers
    for ( int i=0 ; i<nInputs; i++)
    in[i]  = (jack_default_audio_sample_t *)
    jack_port_get_buffer(this->input_port[i], jack_get_buffer_size(client));

    // get output buffers
    for ( int i=0 ; i<nOutputs; i++)
    out[i] = (jack_default_audio_sample_t *)
    jack_port_get_buffer(this->output_port[i], jack_get_buffer_size(client));

    // set output buffer "0.0"
    for(int chanCNT=0; chanCNT<nOutputs; chanCNT++)
    {
        for(int sampCNT=0; sampCNT<nframes; sampCNT++)
        out[chanCNT][sampCNT] = 0.0;
    }

    t_blockIn targetData(in[0], in[0] + nframes);

    bank.fetch(targetData);
    bank.process();
    bank.send(); // test
    // gen.fetch(bank.send());
    // gen.process();

    // mapper.fetch(gen.send());
    // mapper.process();

    // router.fetch(targetData, mapper.send());
    // router.process();

    // output_buffer = router.send();

    // for(int chanCNT=0; chanCNT<nOutputs; chanCNT++){
    //     for(int sampCNT=0; sampCNT<nframes; sampCNT++)
    //     out[chanCNT][sampCNT] = static_cast<jack_default_audio_sample_t>(output_buffer[chanCNT][sampCNT]);
    // }

    return 0;
}