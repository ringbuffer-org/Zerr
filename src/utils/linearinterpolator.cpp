#include "linearinterpolator.h"
using namespace zerr;

void LinearInterpolator::set_value(t_value start, t_value stop, int len){
    start_val = start;
    stop_val  = stop;

    n_steps   = len;
    position = 0;
}

t_value LinearInterpolator::get_value(){
    inter_val = start_val + ((t_value) position * ((stop_val - start_val) / (t_value) (n_steps-1)));

    return inter_val;
}

void LinearInterpolator::next_step(){
    if (position < n_steps) position+=1;
}
