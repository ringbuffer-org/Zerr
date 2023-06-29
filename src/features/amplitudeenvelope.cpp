// #include "utils.h"
// #include "amplitudeenvelope.h"

// using namespace zerr;
// using namespace feature;

// const std::string AmplitudeEnvelope::name        = "Amplitude Envelope";
// const std::string AmplitudeEnvelope::category    = "Time-Domain";
// const std::string AmplitudeEnvelope::description = "The Amplitude Envelope is a representation of how the loudness of an audio signal changes over time";

// void AmplitudeEnvelope::initialize(t_systemConfigs sys_cfg){
//     system_configs = sys_cfg;

//     _reset_param();

//     if (is_initialized()==false){
//         set_initialize_statue(true);
//     }
// }

// void AmplitudeEnvelope::extract(){

//     // int n = x.size();

//     // std::vector<double> envelope(n);
//     // for(int i = 0; i < n; ++i) {
//     //     double sum = 0;
//     //     int count = 0;
//     //     for(int j = std::max(0, i - windowSize); j < std::min(n, i + windowSize); ++j) {
//     //         sum += absSignal[j];
//     //         count++;
//     //     }
//     //     envelope[i] = sum / count;
//     // }

// }

// void AmplitudeEnvelope::reset(){

// }

// void AmplitudeEnvelope::fetch(t_featureInputs in){

//     x = in.wave;

//     for(int i = 0; i < x.size(); ++i) {
//         abs_x[i] = std::abs(x[i]);
//     }
// }

// t_featureValue AmplitudeEnvelope::send(){
    
    
//     return y;
// }

// void AmplitudeEnvelope::_reset_param(){
//     abs_x.resize(AUDIO_BUFFER_SIZE, 0.0f);
//     x.resize(AUDIO_BUFFER_SIZE, 0.0f);
// }
