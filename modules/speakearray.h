#ifndef SPEAKERARRAY_H
#define SPEAKERARRAY_H

#include "zerr.h"
#include "speaker.h"

namespace zerr {
/**
* speaker array class with utilities
*/
class SpeakeArray {
public:
    // params
    int n_speaker;
    std::vector<Speaker> speakers;
    // methods
    Speaker get_speaker_by_index();
    int
    // SpeakeArray();
    // ~SpeakeArray();

private:
    /**
    * Load the 
    */
    void _read_config(std::string fileName);
}; 

} //namespace zerr
#endif // SPEAKERARRAY_H