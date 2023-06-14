#ifndef SPEAKERARRAY_H
#define SPEAKERARRAY_H

#include "utils.h"

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
    /**
    * setup speaker array via the YAML config file
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
    */
    int get_next_one_speaker(int spkr_idx, int mode);
    /**
    * get n random non-duplicated speakers from the speaker array
    */
    std::vector<int> get_contiguous_speakers(int spkr_idx);
    /**
    *  
    */
    std::vector<float> get_distance_vector(int spkr_idx);

private:
    int n_speakers;
    std::vector<int> indexs;
    std::map<int, Speaker> speakers;

    std::vector<std::vector<float>> distance_matrix;

    YAML::Node speaker_config;

    /**
    * Load the YAML speaker config file
    */
    void _init_distance_matrix();
    /**
    * Helping Functions
    */
    std::vector<int> _get_random_indexs(int l, int n);

    float _calculate_distance(Speaker s1, Speaker s2);
}; 

} //namespace zerr
#endif // SPEAKERARRAY_H