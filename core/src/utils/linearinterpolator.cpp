#include "linearinterpolator.h"
using namespace zerr;

void LinearInterpolator::set_value(Param start, Param stop, int len){
    start_val = start;
    stop_val  = stop;

    n_steps   = len;
    position = 0;
}

Param LinearInterpolator::get_value(){
    inter_val = start_val + ((Param) position * ((stop_val - start_val) / (Param) (n_steps-1)));

    return inter_val;
}

void LinearInterpolator::next_step(){
    if (position < n_steps) position+=1;
}
