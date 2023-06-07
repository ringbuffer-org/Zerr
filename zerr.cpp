#include "zerr.h"

#include <stdlib.h>


Zerr::Zerr(SystemConfig sys_cnfg, std::string spkrCfgFile): input_buffer(n_inlet, std::vector<float>(sys_cnfg.block_size, 0.0f)){
    // zerr_cfg = zerrCfgFile;
    spkr_cfg = spkrCfgFile;

    // in_buf = new RingBuffer(2048);

    bank   = new zerr::FeatureBank();
    gen    = new zerr::TrajectoryGenerator();
    mapper = new zerr::Mapper();
    router = new zerr::AudioRouter();

    sample_rate = sys_cnfg.sample_rate;
    block_size  = sys_cnfg.block_size;
}
 
void Zerr::initialize(){
    in_buf = new RingBuffer(fft_size);
    // try constexper
    bank->regist_all();

    // replace with inside one time setup function
    zerr::str_vec feature_names = {"ZeroCrossingRate", "Centroid"}; //  Centroid
    for (auto name : feature_names) {
        bank->setup(name);
    }

    // bank.print_all_features();
    // bank.print_active_features();

    bank->initialize();
    gen->initialize();
    mapper->initialize(spkr_cfg);
    router->initialize(block_size, mapper->get_n_speaker() + 1); 

    n_outlet = mapper->get_n_speaker();
    // post("Zerr::initialize: ");
    post(std::to_string(mapper->get_n_speaker()).c_str());

    input_buffer.resize(n_inlet, std::vector<float>(block_size, 0.0f));
    output_buffer.resize(n_outlet, std::vector<float>(block_size, 0.0f));

    in_ptr  = (float **) getbytes(n_inlet * sizeof(float **));
    out_ptr = (float **) getbytes(n_outlet * sizeof(float **));
}

void Zerr::perform(float **in, float **out, int n_vec){
    post("Zerr::perform");
    // zerr::input_vec targetData(in[0], in[0] + block_size);
    // for (int i = 0; i < n; ++i){
    //     input_buffer[i] = in[i];
    // }
    

    // bank->fetch(input_buffer);
    // bank->process();
    // gen->fetch(bank->send());
    // gen->process();

    // mapper->fetch(gen->send());
    // mapper->process();

    // router->fetch(input_buffer, mapper->send());

    // router->process();

    // output_buffer = router->send();
    // // post("Zerr::perform...");
    // // post(std::to_string(n).c_str());
    // // post(std::to_string(block_size).c_str());

    // for (int i=0; i<n; i++){
    //     out[i] = output_buffer[0][i];
    //     // out[i] = 0.5;
    //     // in[i] = -0.5;
    // }
}

void Zerr::pd_perform(float **ports, int n_vec){


    in_ptr  = (float **) &ports[0];
    out_ptr = (float **) &ports[n_inlet];

    for (int j = 0; j < n_vec; j++) {
        in_buf->enqueue(in_ptr[0][j]);
    }

    in_buf->getBufferSamples();
    // post("Zerr::pd_perform");
    for (int i = 0; i < n_inlet; i++) {
        for (int j = 0; j < n_vec; j++) {
            input_buffer[i][j] = in_ptr[i][j];
        }
    }

    bank->fetch(input_buffer[0]);
    bank->process();
    gen->fetch(bank->send());
    gen->process();

    mapper->fetch(gen->send());
    mapper->process();

    router->fetch(input_buffer[0], mapper->send());

    router->process();

    output_buffer = router->send();

    for (int i = 0; i < n_outlet; i++) {
        for (int j = 0; j < n_vec; j++) {
            out_ptr[i][j] = output_buffer[i][j];
        }
    }
}

int Zerr::get_port_count(){
    return n_inlet+n_outlet;
}

Zerr::~Zerr(){
    delete bank;
    delete gen;
    delete mapper;
    delete router;
}
