#ifndef MAPPER_H
#define MAPPER_H

#include "zerr.h"
#include "speakerarray.h"

namespace zerr {
/**
* This class applies mapping strategy based on input control signal and the speaker array setups
* For demonstraction now only the trigger jump implemented
*/
class Mapper {
public:
    void initialize(std::string config_path);
    void fetch(float in);
    void process();
    std::vector<float> send();
    void reset();

private:
    /**
    * The 
    */
    SpeakerArray speaker_array;
    /**
    * The mapping vector e.g: [0, 1, 0.5, 0.8, 0.8, 0, 0, 0]
    * 
    */
    std::vector<float> mapping; 
    void _init_mapping(int n);
    void _update_mapping();
    void _print_mapping(std::string note);
    /**
    * input control signal
    * TODO: use a ringbuffer instead of single float number
    */
    float x;
    int prev_x;
    /**
    * temp params to achive the speaker jump
    */
    int curr_idx; // set to virtual point
}; 

} //namespace zerr
#endif // MAPPER_H