#include "linearinterpolator.h"
using namespace zerr;

void LinearInterpolator::initialize(int size){
    output_size = size;
    inter_val.resize(output_size);
    start_val.resize(output_size);
    stop_val.resize(output_size);

}

void LinearInterpolator::set_values(t_volumes start, t_volumes stop, int len){
    start_val = start;
    stop_val  = stop;

    n_steps   = len;
    position = 0;
}

t_volumes LinearInterpolator::get_value(){
    for (int i = 0; i < output_size; ++i){
        inter_val[i] = start_val[i] + ((t_value) position * ((stop_val[i] - start_val[i]) / (t_value) (n_steps-1)));
    }
            
    return inter_val;
}

void LinearInterpolator::next_step(){
    if (position < n_steps) position+=1;
}
