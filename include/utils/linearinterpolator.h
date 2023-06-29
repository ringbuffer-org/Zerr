#ifndef LINEARINTERPOLATOR_H
#define LINEARINTERPOLATOR_H

#include "configs.h"
#include "utils.h"

namespace zerr{

// class LinearInterpolator{
// public:
//     void      initialize(int size);
//     void      set_values(t_volumes start, t_volumes stop, int len);
//     t_volumes get_value();
//     void      next_step();


// private:
//     t_volumes inter_val;
//     t_volumes start_val;
//     t_volumes stop_val;

//     int n_steps;
//     int position;
//     int output_size;
// };

class LinearInterpolator{
public:
    void    set_value(t_value start, t_value stop, int len);
    t_value get_value();
    void    next_step();

private:
    t_value inter_val;
    t_value start_val;
    t_value stop_val;

    int n_steps;
    int position;
};

} // zerr
#endif // LINEARINTERPOLATOR_H
