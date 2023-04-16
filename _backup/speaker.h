#ifndef SPEAKER_H
#define SPEAKER_H

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

} //namespace zerr
#endif // SPEAKER_H