#include "zerr.h"
#include "zerocrossing.h"

using namespace zerr;
using namespace feature;

const std::string ZeroCrossing::name        = "Zero Crossing";
const std::string ZeroCrossing::category    = "Time-Domain";
const std::string ZeroCrossing::description = "This detects whether the sign of incoming sample changed. Output 1 when change detected";

void ZeroCrossing::initialize(){
    pre_x     = 0;
    if (is_initialized()==false){
        set_initialize_statue(true);
    }
}

void ZeroCrossing::extract(){
    new_x = x[0];

    // std::cout<<"new_x: "<<new_x<<" | pre_x: "<<pre_x<<std::endl;

    sign1 =   (new_x > 0) - (new_x < 0);
    sign2 =   (pre_x > 0) - (pre_x < 0);

    pre_x = new_x;
    y = ((sign1 * sign2) < 0);
}

void ZeroCrossing::reset(){
    pre_x     = 0;
    std::cout<<"ZeroCrossing::reset"<<std::endl;
}

void ZeroCrossing::fetch(std::vector <double> in){
    x.clear();
    x = in;
}

float ZeroCrossing::send(){
    return static_cast<float>(y);
}


