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

#include<jack/jack.h>

namespace zerr{

class Zerr{
public:
    /**
    * setup configs files and 
    */
    Zerr(std::string zerrCfgFile, std::string spkrCfgFile);
    /**
    * initialize all zerr modules and the audio clients
    */
    void initialize();
    /**
    * activate the audioclient and zerr callback
    * run it forever till user interrupted
    */
    void run();

private:
    //basic config
    // int sr = 48000;
    int frame_size = 1024; //tmp
    std::vector<t_blockIn> output_buffer;

    // config path
    std::string zerr_cfg;
    std::string spkr_cfg;

    //module objects
    FeatureBank bank;
    TrajectoryGenerator gen;
    Mapper mapper;
    AudioRouter router;

    //jackclient
    int nInputs = 1;
    int nOutputs = 8;
    jack_client_t   *client;
    jack_status_t   status;
    jack_port_t     **input_port;
    jack_port_t     **output_port;
    jack_default_audio_sample_t **in, **out;
    static int callback_process(jack_nframes_t x, void* object);
    int process (jack_nframes_t nframes);

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
