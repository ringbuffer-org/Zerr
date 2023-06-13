#ifndef MAPPER_H
#define MAPPER_H

#include "utils.h"
#include "speakerarray.h"

namespace zerr {
/**
* This class applies mapping strategy based on input control signal and the speaker array setups
* For demonstraction now only the trigger jump implemented
*/
class Mapper {
public:
    void initialize(std::string config_path);
    void fetch(t_featureValueList in);
    void process();
    std::vector<t_value> send();
    void reset();

    int get_n_speaker();

private:
    /**
    * The 
    */
    SpeakerArray speaker_array;
    /**
    * The mapping vector e.g: [0, 1, 0.5, 0.8, 0.8, 0, 0, 0]
    * 
    */
    std::vector<t_value> mapping; 
    void _init_mapping(int n);
    void _update_mapping();
    void _print_mapping(std::string note);
    /**
    * input control signal
    */
    t_featureValueList x;
    t_featureValueList prev_x;
    /**
    * temp params to achive the speaker jump
    */
    int cold_down_time;
    int curr_idx; // set to virtual point
    int jump_cnt;
}; 

} //namespace zerr
#endif // MAPPER_H