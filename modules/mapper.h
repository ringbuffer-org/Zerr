#ifndef MAPPER_H
#define MAPPER_H

#include "zerr.h"
#include "speakerarry.h"

namespace zerr {
/**
* This class applies mapping strategy based on input
*/
class Mapper {
public:
    

private:
    /**
    * This class merges outputs of feature extractors to generate control 
    */
    unique_ptr<SpeakerArray> spk_arry;
    unique_ptr<TrajectoryGenerator> trjct_gen;


}; 

} //namespace zerr
#endif // MAPPER_H