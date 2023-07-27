/**
 * @file envelopegenerator.h 
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Envelope Generator Class Implementation
 * @version 0.4
 * @date 2023-07-27
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef ENVELOPEGENERATOR_H
#define ENVELOPEGENERATOR_H

#include "utils.h"
#include "types.h"
#include "speakerarray.h"

namespace zerr {
/**
* EnvelopeGenerator applies mapping strategy based on input control signal and the speaker array setups
*/
class EnvelopeGenerator { 
public:
    void initialize(std::string config_path);
    void fetch(t_featureValueList in);
    void process();
    std::vector<t_value> send();
    void reset();

    int get_n_speaker();

private:
    /**
    * The SpeakerArray
    */
    SpeakerArray speaker_array;
    /**
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
    t_value volume;
    t_value trigger;
    t_value floating;
    t_value width;
    /**
    * temp params to achive the speaker jump
    */
    int cold_down_time;
    int curr_idx; // set to virtual point
    int jump_cnt;

    t_value _calculate_normal_distribution(t_value x, t_value alpha);
}; 

} //namespace zerr
#endif // ENVELOPEGENERATOR_H