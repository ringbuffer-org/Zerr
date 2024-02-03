#include "featureprocessor.h"
using namespace zerr;

void FeatureProcessor::initialize(int N, std::string mode){
    output_mode = mode;
    n_features  = N;
    if (output_mode == "bypass"){
        y.resize(n_features);
    }
}

void FeatureProcessor::fetch(t_featureValueList in){
    x.clear();
    x = in;
    assert(x.size() == n_features);
}

void FeatureProcessor::process(){
    if (output_mode == "bypass") y = x;
}

t_featureValueList FeatureProcessor::send(){
    return y;
}
