#ifndef SPEAKERARRAY_H
#define SPEAKERARRAY_H

#include "zerr.h"
// #include "speaker.h"

namespace zerr {
/**
* structure of speaker entity
*/
typedef struct Speaker {
    int index;
    float x;
    float y;
    float z;
    float directness;
    std::vector<int> contiguous;
} Speaker;

/**
* speaker array class with utilities for manipulating speakers
*/
class SpeakerArray {
public:
    // params
    std::vector<Speaker> speakers;
    int n_speaker;
    std::vector<int> speaker_indexs; //map between vector index and speaker index
    std::map<int, std::vector<int>> contiguous;

    // methods
    // SpeakeArray();
    /**
    * setup speaker array via the YAML config file
    */
    void initialize(std::string config_path);
    /**
    * return the speaker structure with selected index
    */
    Speaker get_speaker_by_index(int spkr_idx);
    /**
    * get one random speaker from the speaker array
    */
    int get_random_one_speaker();
    /**
    * get n random non-duplicated speakers from the speaker array
    */
    std::vector<int> get_random_speakers(int n);
    /**
    * return one contigate speaker in the contiguous matrix. Three modes supported:
    * nearest: return the speaker with lowest weight
    * farest: return the speaker with highest weight
    * random: return a random one. 
    * TODO: is it playful to assign the probability based on normalised weight?
    * Only non-weighted graph supported for now
    */
    int get_next_one_speaker(int spkr_idx, std::string mode);

    std::vector<int> get_connected_speakers(int spkr_idx)
    // 
    // ~SpeakeArray();

private:
    YAML::Node speaker_config;
    /**
    * Load the YAML speaker config file
    */
    void _read_config(std::string config_path);
    void _init_contiguous_matrix();
    void _init_speaker_indexs_map();
}; 

} //namespace zerr
#endif // SPEAKERARRAY_H