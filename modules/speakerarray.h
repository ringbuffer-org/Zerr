#ifndef SPEAKERARRAY_H
#define SPEAKERARRAY_H

#include "zerr.h"

namespace zerr {
/**
* structure of speaker entity
*/
class Speaker {
public:
    Speaker(int idx, float x, float y, float z, float directness, std::vector<int> contiguous);

    // all the getters
    float get_idx(){return idx;};

    float get_x(){return x;};
    float get_y(){return y;};
    float get_z(){return z;};
    float get_directness(){return directness;};

    int get_contiguous_len(){return contiguous.size();};
    std::vector<int> get_contiguous(){return contiguous;};

    //Print out configs to console
    void print();

private:
    int idx;
    float x;
    float y;
    float z;
    float directness;
    std::vector<int> contiguous;
} ;

/**
* speaker array class with utilities for manipulating speakers
*/
class SpeakerArray {
public:
    // SpeakeArray();
    /**
    * setup speaker array via the YAML config file
    * TODO: check the YAML file sanity
    */
    void initialize(std::string config_path);
    /**
    * get the number of speakers in this speaker array setup
    */
    int get_n_speakers(); 
    /**
    * return the speaker class with specific index
    */
    Speaker get_speaker_by_index(int spkr_idx);
    /**
    * get n random non-duplicated speakers from the speaker array
    * TODO: non-duplicated
    */
    std::vector<int> get_random_speakers(int spkr_idx, int n);
    /**
    * return one contigate speaker in the contiguous matrix. Three modes supported:
    * random(0): return a random one. 
    * mono(1): return the only one that connected 
    * nearest(2): return the speaker with lowest weight(eculian distance for now)
    * TODO: is it playful to assign the probability based on normalised weight?
    * TODO: add type.h file
    * Only non-weighted graph supported for now
    */
    int get_next_one_speaker(int spkr_idx, int mode);
    /**
    * get n random non-duplicated speakers from the speaker array
    */
    std::vector<int> get_contiguous_speakers(int spkr_idx);
    // 
    // ~SpeakeArray();

private:
    int n_speakers;
    std::vector<int> indexs;
    std::map<int, Speaker> speakers; //map between vector index and speaker index
    std::map<int, std::vector<int>> contiguous;

    YAML::Node speaker_config;

    /**
    * Load the YAML speaker config file
    */
    void read_config(std::string config_path);
    void init_contiguous_matrix();
    void init_speaker_indexs_map();
    /**
    * Helping Functions
    */
    std::vector<int> get_random_indexs(int l, int n);
}; 

} //namespace zerr
#endif // SPEAKERARRAY_H