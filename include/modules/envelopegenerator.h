/**
 * @file envelopegenerator.h 
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Envelope Generator Class Implementation
 * @version 0.5
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef ENVELOPEGENERATOR_H
#define ENVELOPEGENERATOR_H

// #include "utils.h"
#include "types.h"
#include "speakermanager.h"

namespace zerr {
/**
* @brief EnvelopeGenerator generates evelope stream according to input control signal and the speaker array setups
*/
class EnvelopeGenerator { 
public:
    EnvelopeGenerator(std::string spkr_cfg, std::string mode);







    bool initialize();
    void fetch(t_featureValueList in);
    void process();
    t_blockOut send();
    void reset();

private:
    SpeakerManager *speaker_manager; /**< SpeakerManager object inside */
    std::string speaker_config;
    std::string mode;










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

    t_value _calculate_normal_distribution(t_value x, t_value alpha);
}; 

} //namespace zerr
#endif // ENVELOPEGENERATOR_H