#ifndef ZERR_H
#define ZERR_H

// dependencies
#include "utils.h"
#include "types.h"

// modules
#include "audiorouter.h"
#include "featurebank.h"
#include "mapper.h"
#include "trajectorygenerator.h"

namespace zerr{

class Zerr{
public:
    /**
    * setup configs files and 
    */
    Zerr(std::string zerrCfgFile, std::string spkrCfgFile);
    /**
    * initialize all zerr components and the audio clients
    */
    void initialize();
    /**
    * 
    */
    void run();

private:
    //basic config
    int sr = 48000;
    int frame_size = 1024; //tmp

    // config path
    std::string zerr_cfg;
    std::string spkr_cfg;

    //module objects
    FeatureBank bank;
    TrajectoryGenerator gen;
    Mapper mapper;
    AudioRouter router;

    /**
    * seperarate the initialization of zerr modules and audio client
    */
    void _initialize_zerr();
    void _initialize_audioclient();
    /**
    * hold the run function
    */
    void _hold();
};

}  //namespace zerr
#endif //ZERR_H
