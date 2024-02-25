#include "trajectorygenerator.h"
using namespace zerr;

void TrajectoryGenerator::initialize(int N, std::string mode){
    output_mode = mode;
    n_features  = N;
    if (output_mode == "bypass"){
        y.resize(n_features);
    }
}

void TrajectoryGenerator::fetch(t_featureValueList in){
    x.clear();
    x = in;
    assert(x.size() == n_features);
}

void TrajectoryGenerator::process(){
    if (output_mode == "bypass") y = x;
}

t_featureValueList TrajectoryGenerator::send(){

    #ifdef TESTMODE
    // std::cout<<"TrajectoryGenerator::send  "<<y[0].normalized<<"  "<<y[1].normalized<<std::endl;
    #endif // TESTMODE

    return y;
}




t_value TrajectoryGenerator::weight_add(std::vector<t_value> weights){
    t_value res = 0;
    for (int i = 0; i < x.size(); ++i){
        res = res + ( x[i].original * weights[i] );
    }
    return res;
}
