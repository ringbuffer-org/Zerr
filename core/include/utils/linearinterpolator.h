#ifndef LINEARINTERPOLATOR_H
#define LINEARINTERPOLATOR_H

#include "configs.h"
#include "utils.h"

namespace zerr{

class LinearInterpolator{
public:
    void    set_value(Param start, Param stop, int len);
    Param get_value();
    void    next_step();

private:
    Param inter_val;
    Param start_val;
    Param stop_val;

    int n_steps;
    int position;
};

} // zerr

#endif // LINEARINTERPOLATOR_H
