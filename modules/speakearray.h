#ifndef SPEAKERARRAY_H
#define SPEAKERARRAY_H

#include "zerr.h"
#include "speaker.h"

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
    std::map<int, std::vector<int>> contiguous;

    // methods
    // SpeakeArray();
    void initialize(std::string config_path);
    Speaker get_speaker_by_index(int speaker_index);
    /**
    * get one random speaker from the speaker array
    */
    int get_random_one_speaker();
    /**
    * get n random non-duplicated speakers from the speaker array
    */
    std::vector<int> get_random_speakers(int n);
    int get_next_speaker(int speaker_index);
    int get_
    // 
    // ~SpeakeArray();

private:
    YAML::Node speaker_config;
    /**
    * Load the YAML speaker config file
    */
    void _read_config(std::string config_path);
    void _init_contiguous_matrix();
}; 

} //namespace zerr
#endif // SPEAKERARRAY_H