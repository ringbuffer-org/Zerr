#include "trajectorygenerator.h"
using namespace zerr;

void TrajectoryGenerator::initialize(){
    //tmp
    // std::cout<<"initialized!"<<std::endl;
}

void TrajectoryGenerator::fetch(t_featureValueList in){
    //tmp
    x.clear();
    x = in;
}

void TrajectoryGenerator::process(){
    // messy temparary process function
    // two input channels are weighted and added then compared with a constant threshold
    // the output 
    std::vector<t_value> weights = {0.5, 0.5};
    y = weight_add(weights);
    // y = (tmp > 0.5);
    std::cout<<"merged!: "<<y<<std::endl;
}

t_value TrajectoryGenerator::send(){
    return y;
}

t_value TrajectoryGenerator::weight_add(std::vector<t_value> weights){
    t_value res = 0;
    for (int i = 0; i < x.size(); ++i)
    {
        res = res + ( x[i].original * weights[i] );
    }
    return res;
}
