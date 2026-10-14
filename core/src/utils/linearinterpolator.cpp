#include "linearinterpolator.h"
using namespace zerr;

void LinearInterpolator::set_value(Param start, Param stop, int len)
{
    start_val = start;
    stop_val  = stop;

    n_steps  = len;
    position = 0;
}

Param LinearInterpolator::get_value()
{
    // A single step has no slope to interpolate along (block size 1 is legal in Pd)
    if (n_steps <= 1) {
        inter_val = stop_val;
        return inter_val;
    }

    inter_val = start_val + (static_cast<Param>(position) *
                             ((stop_val - start_val) / static_cast<Param>(n_steps - 1)));

    return inter_val;
}

void LinearInterpolator::next_step()
{
    if (position < n_steps)
        position += 1;
}
