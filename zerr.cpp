#include "zerr.h"

#include <stdlib.h>

// using namespace zerr;



Zerr::Zerr(SystemConfig sys_cnfg, std::string spkrCfgFile): input_buffer(sys_cnfg.block_size, 0.0f){
    // zerr_cfg = zerrCfgFile;
    spkr_cfg = spkrCfgFile;

    bank   = new zerr::FeatureBank();
    gen    = new zerr::TrajectoryGenerator();
    mapper = new zerr::Mapper();
    router = new zerr::AudioRouter();

    sample_rate = sys_cnfg.sample_rate;
    block_size  = sys_cnfg.block_size;
}
 
void Zerr::initialize(){
    // try constexper
    bank->regist_all();

    // replace with inside one time setup function
    zerr::str_vec feature_names = {"ZeroCrossingRate", "ZeroCrossingRate"}; //, "ZeroCrossingRate"  Centroid
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
    post("Zerr::initialize: ");
    post(std::to_string(mapper->get_n_speaker()).c_str());
}

void Zerr::perform(float *in, float *out, int n){
    // post("Zerr::in: ");

    // zerr::input_vec targetData(in[0], in[0] + block_size);
    for (int i = 0; i < n; ++i){
        input_buffer[i] = in[i];
    }
    

    bank->fetch(input_buffer);
    bank->process();
    gen->fetch(bank->send());
    gen->process();

    mapper->fetch(gen->send());
    mapper->process();

    router->fetch(input_buffer, mapper->send());

    router->process();

    output_buffer = router->send();
    post("Zerr::perform...");
    post(std::to_string(n).c_str());
    post(std::to_string(block_size).c_str());

    for (int i=0; i<n; i++){
        // out[i] = output_buffer[0][i];
        out[i] = 0.5;
    }
}

Zerr::~Zerr(){
    delete bank;
    delete gen;
    delete mapper;
    delete router;
}




















// #ifdef __cplusplus
// extern "C" {
// #endif

// zerr_pd *zerr_new(sys_config *config) {
//     zerr_pd *z = (zerr_pd *) malloc(sizeof(zerr_pd));
//     if (!z) return NULL;
//     z->n_outlet = 6;
//     memcpy(&z->cfg, config, sizeof(sys_config));

//     return z;
// }

// void zerr_free(zerr_pd *z) {
//     free(z);
// }

// void zerr_perform(zerr_pd *z, float *in, float *out, int n) {
//     post("Hello world !!");
// }

// #ifdef __cplusplus
// }
// #endif
