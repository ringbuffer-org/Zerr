#include "onset.h"
#include "zerr.h"
using namespace zerr;
using namespace feature;

const std::string Onset::name        = "Onset";
const std::string Onset::category    = "spectrum";
const std::string Onset::description = "The spectral centroid is a measure used in digital signal processing to characterise a spectrum.";

void Onset::initialize(){
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void Onset::extract(){
    std::cout<<"Onset: extract"<<std::endl;
}

void Onset::reset(){
    std::cout<<"Onset::reset"<<std::endl;
}

void Onset::fetch(std::vector <double> in){
    x.clear();
    x = in;
}

float Onset::send(){
    return y;
}
