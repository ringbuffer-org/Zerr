#include "centroid.h"
// #include "zerr.h"
using namespace zerr;

// Centroid::Centroid(): FeatureExtractor() {
//     name = "Spectrum Centroid"; 
//     // name = "Spectrum Centroid"; 
//     // description = "The spectral centroid is a measure used in digital signal processing to characterise a spectrum. It indicates where the center of mass of the spectrum is located. Perceptually, it has a robust connection with the impression of brightness of a sound.[1] It is sometimes called center of spectral mass."; 
//     // category = "spectrum";
//     // std::cout<<"Centroid constructed!"<<std::endl;
// }

std::string Centroid::get_name(){
    return name;
}