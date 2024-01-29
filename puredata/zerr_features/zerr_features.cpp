#include "zerr_features.h"

#include <stdlib.h>

ZerrFeatureTracker::ZerrFeatureTracker(zerr::t_systemConfigs sys_cnfg, zerr::t_featureNames ft_names): input_buffer(n_inlet, std::vector<double>(sys_cnfg.block_size, 0.0f)){

    bank   = new zerr::FeatureBank();

    system_configs.sample_rate = sys_cnfg.sample_rate;
    system_configs.block_size  = sys_cnfg.block_size;

    for (int i = 0; i < ft_names.num; ++i){
        feature_names.push_back(ft_names.names[i]);
    }
}


int ZerrFeatureTracker::initialize(){

    try{
        bank->initialize(feature_names, system_configs);
    }catch (...) {
        return 0;
    }
    
    n_outlet = feature_names.size();

    input_buffer.resize(n_inlet, std::vector<double>(system_configs.block_size, 0.0f));
    output_buffer.resize(n_outlet);

    in_ptr  = (float **) malloc(n_inlet * sizeof(float **));
    out_ptr = (float **) malloc(n_outlet * sizeof(float **));

    return 1;
}


void ZerrFeatureTracker::perform(float **ports, int n_vec){

    in_ptr  = (float **) &ports[0];
    out_ptr = (float **) &ports[n_inlet];

    for (int i = 0; i < n_inlet; i++) {
        for (int j = 0; j < n_vec; j++) {
            input_buffer[i][j] = in_ptr[i][j];
        }
    }

    bank->fetch(input_buffer[0]);
    bank->process();
    output_buffer = bank->send();

    for (int i = 0; i < n_outlet; i++) {
        for (int j = 0; j < n_vec; j++) {
            out_ptr[i][j] = output_buffer[i][j];
        }
    }

}


int ZerrFeatureTracker::get_port_count(){
    return n_inlet+n_outlet;
}


ZerrFeatureTracker::~ZerrFeatureTracker(){
    delete bank;
}
