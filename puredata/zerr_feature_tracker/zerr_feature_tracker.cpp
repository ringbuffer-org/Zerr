#include "zerr_feature_tracker.h"

#include <stdlib.h>

#include "m_pd.h" //PureData APIs

ZerrFeatureTracker::ZerrFeatureTracker(t_systemConfigs sys_cnfg, std::string spkrCfgFile): input_buffer(n_inlet, std::vector<double>(sys_cnfg.block_size, 0.0f)){
    // zerr_cfg = zerrCfgFile;
    spkr_cfg = spkrCfgFile;

    bank   = new zerr::FeatureBank();

    system_configs.sample_rate = sys_cnfg.sample_rate;
    system_configs.block_size  = sys_cnfg.block_size;
}
 
void ZerrFeatureTracker::initialize(){
    // replace with inside one time setup function
    zerr::t_featureNameList feature_names = {"ZeroCrossingRate", "RMSAmplitude", "Centroid"}; //  Centroid

    bank->initialize(feature_names, system_configs);

    n_outlet = feature_names.size()*2;

    input_buffer.resize(n_inlet, std::vector<double>(system_configs.block_size, 0.0f));
    output_buffer.resize(n_outlet);

    in_ptr  = (float **) getbytes(n_inlet * sizeof(float **));
    out_ptr = (float **) getbytes(n_outlet * sizeof(float **));
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
            out_ptr[i][j] = (i%2==0)?output_buffer[i/2].original:output_buffer[i/2].normalized;
        }
    }

}

int ZerrFeatureTracker::get_port_count(){
    return n_inlet+n_outlet;
}

ZerrFeatureTracker::~ZerrFeatureTracker(){
    delete bank;
}
