#include "trajectorygenerator.h"
using namespace zerr;

void TrajectoryGenerator::initialize(){
    //tmp
    std::cout<<"initialized!"<<std::endl;
}

void TrajectoryGenerator::fetch(std::vector<float> in){
    //tmp
    xs.clear();
    xs = in;
    std::cout<<"updated!"<<std::endl;
}

void TrajectoryGenerator::merge(){
    // messy temparary merge function
    float tmp;
    std::vector<float> weights = {0.5, 0.1};
    tmp = weight_add(xs, weights);
    y = (tmp > 0.5);
    std::cout<<"merged!: "<<tmp<<std::endl;
}

float TrajectoryGenerator::send(){
    return y;
}

float TrajectoryGenerator::weight_add(std::vector<float> xs, std::vector<float> weights){
    float res = 0;
    for (int i = 0; i < xs.size(); ++i)
    {
        res = res + ( xs[i] * weights[i] );
    }
    return res;
}
