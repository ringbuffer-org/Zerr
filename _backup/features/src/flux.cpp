// #include "flux.h"
// #include "zerr.h"
// using namespace zerr;
// using namespace feature;

// const std::string Flux::name        = "Flux";
// const std::string Flux::category    = "spectrum";
// const std::string Flux::description = "The spectral centroid is a measure used in digital signal processing to characterise a spectrum.";

// void Flux::initialize(){
//     _reset_param();
//     if (is_initialized()==false){
//         set_initialize_statue(true);
//     }
//     std::cout<<"Feature:Flux is initialized!"<<std::endl;
// }

// void Flux::extract(){
//     _reset_param();
//     // get new power spectrum
//     for(int tmpCNT = 0; tmpCNT<n_fft; tmpCNT++)
//     {

//         if(power_spectrum[tmpCNT]>spec_max)
//             spec_max = power_spectrum[tmpCNT];

//         if(last_spec_max>0)
//             dist += pow((power_spectrum[tmpCNT] - last_power_spectrum[tmpCNT]) / last_spec_max ,2);

//         // remember last power spectrum value
//         last_power_spectrum[tmpCNT] = power_spectrum[tmpCNT];
//     }
//     last_spec_max = spec_max;
// }

// void Centroid::_reset_param(){
//     spec_max = 0;
//     dist     = 0;
// }