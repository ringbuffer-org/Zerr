#include "zerr.h"
#include "centroid.h"

using namespace zerr;
using namespace feature;

const std::string Centroid::name        = "Centroid";
const std::string Centroid::category    = "spectrum";
const std::string Centroid::description = "The spectral centroid is a measure used in digital signal processing to characterise a spectrum.";

void Centroid::initialize(){
    _reset_param();
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void Centroid::extract(){
    //reset the parameters
    _reset_param();
    // from k=1 to K:
    for(auto it = std::next(begin(x)); it != std::end(x); ++it)
    {
        num   += k* *it;
        denum += *it;

        k+=1;
    }

    y = (num / denum) / (float) x.size();
}

void Centroid::reset(){
    std::cout<<"Centroid::reset"<<std::endl;
}

void Centroid::fetch(std::vector <double> in){
    x.clear();
    x = in;
}

float Centroid::send(){
    return y;
}

void Centroid::_reset_param(){
    k     = 0;
    denum = 0.0000;
    num   = 0.00001;
}

