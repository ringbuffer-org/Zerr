#include "zerr_speaker_mapper.h"

#include <stdlib.h>

// using namspace zerr_speaker_mapper;


ZerrSpeakerMapper::ZerrSpeakerMapper(t_systemConfigs sys_cnfg, std::string spkrCfgFile): input_buffer(n_inlet, std::vector<double>(sys_cnfg.block_size, 0.0f)){
    spkr_cfg = spkrCfgFile;

    mapper = new zerr::Mapper();

    system_configs.sample_rate = sys_cnfg.sample_rate;
    system_configs.block_size  = sys_cnfg.block_size;
}
 
void ZerrSpeakerMapper::initialize(){

    // replace with inside one time setup function
    // zerr::t_featureNameList feature_names = {"ZeroCrossingRate", "RMSAmplitude", "Centroid"}; //  Centroid


    // bank->initialize(feature_names, system_configs);
    // gen->initialize(feature_names.size(), "bypass");
    mapper->initialize(spkr_cfg);
    // router->initialize(system_configs.block_size, mapper->get_n_speaker()); 

    n_outlet = mapper->get_n_speaker();
    // post("Zerr::initialize: ");
    post(std::to_string(mapper->get_n_speaker()).c_str());

    input_buffer.resize(n_inlet, std::vector<double>(system_configs.block_size, 0.0f));
    output_buffer.resize(n_outlet, std::vector<double>(system_configs.block_size, 0.0f));

    in_ptr  = (float **) getbytes(n_inlet * sizeof(float **));
    out_ptr = (float **) getbytes(n_outlet * sizeof(float **));
}

// void Zerr::perform(float **in, float **out, int n_vec){
//     post("Zerr::perform");
// }

void ZerrSpeakerMapper::perform(float **ports, int n_vec){


    in_ptr  = (float **) &ports[0];
    out_ptr = (float **) &ports[n_inlet];

    for (int i = 0; i < n_inlet; i++) {
        for (int j = 0; j < n_vec; j++) {
            input_buffer[i][j] = in_ptr[i][j];
        }
    }

    mapper->fetch(input_buffer); //buggy
    mapper->process();
    output_buffer = mapper->send();

    for (int i = 0; i < n_outlet; i++) {
        for (int j = 0; j < n_vec; j++) {
            out_ptr[i][j] = output_buffer[i][j];
        }
    }
}

int ZerrSpeakerMapper::get_port_count(){
    return n_inlet+n_outlet;
}

ZerrSpeakerMapper::~ZerrSpeakerMapper(){
    // delete bank;
    // delete gen;
    delete mapper;
    // delete router;
}
