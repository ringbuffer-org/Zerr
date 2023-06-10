#include "utils.h"
#include "zerocrossing.h"

using namespace zerr;
using namespace feature;

const std::string ZeroCrossingRate::name        = "Zero Crossing Rate";
const std::string ZeroCrossingRate::category    = "Time-Domain";
const std::string ZeroCrossingRate::description = "The zero crossing rate (ZCR) is a measure of how frequently a signal changes its sign. It represents the rate at which the signal crosses the zero amplitude level over a given time period.";

void ZeroCrossingRate::initialize(){
    zero_crossings     = 0;
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void ZeroCrossingRate::extract(){
    assert(is_initialized());
    zero_crossings = 0;
    for (size_t i = 1; i < x.size(); ++i) {
        if ((x[i] >= 0 && x[i - 1] < 0) || (x[i] < 0 && x[i - 1] >= 0)) {
            ++zero_crossings;
        }
    }

    y = static_cast<float>(zero_crossings) / (x.size() - 1);
}

void ZeroCrossingRate::reset(){
    std::cout<<"ZeroCrossingRate::reset"<<std::endl;
}

void ZeroCrossingRate::fetch(std::vector <float> in){
    x.clear();
    x = in;
}

float ZeroCrossingRate::send(){
    return static_cast<float>(y);
}


