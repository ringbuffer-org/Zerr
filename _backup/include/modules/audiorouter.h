#ifndef AUDIOROUTRT_H
#define AUDIOROUTRT_H

#include "utils.h"

namespace zerr {
/**
* This class will receive input audio signals and routing the signal based on the rules from Mapper
*/
class AudioRouter {
public:
    // typedef std::vector<double> audio_vec;
    typedef std::vector<float>  cntrl_vec;
    typedef std::vector<input_vec> out_mat;

    void initialize(int num_audio, int num_cntrl);
    void fetch(input_vec audio_in, cntrl_vec cntrl_in);
    void process();
    out_mat send();
    void reset();

private:
    input_vec audio_x;
    cntrl_vec cntrl_x;
    out_mat ys;
}; 

} //namespace zerr
#endif // AUDIOROUTRT_H