#ifndef AUDIOROUTRT_H
#define AUDIOROUTRT_H

#include "utils.h"
#include "linearinterpolator.h"

namespace zerr {
/**
* This class will receive input audio signals and routing the signal based on the rules from Mapper
*/
class AudioRouter {
public:
    void initialize(int block_size, int num_channels);
    void fetch(t_blockIn audio_in, t_volumes cntrl_in);
    void process();
    t_blockOuts send();
    void reset();

private:
    t_blockIn audio_x;
    t_volumes cntrl_x;

    t_blockOuts y;

    t_value main_volume;

    LinearInterpolator linear_interpolator;
    t_volumes cntrl_x_prev;
}; 

} //namespace zerr
#endif // AUDIOROUTRT_H