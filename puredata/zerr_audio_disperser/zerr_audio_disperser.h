/**
 * @file zerr_audio_disperser.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief AudioDisperser Class Puredata Wrapper
 * @version 0.4
 * @date 2023-08-04
 * 
 * @copyright Copyright (c) 2023
 */
#pragma once
// standard 

// utils
#include "types.h"
#include "utils.h"
// #include "ringbuffer.h"
#include "logger.h"
// modules
#include "audiodisperser.h"

class ZerrAudioDisperser{
public:
    int numInlet;     /**< number of inlets*/
    int numOutlet;    /**< number of outlets*/
    /**
    * @brief create a new ZerrAudioDisperser instance 
    * @param numChannel the number of channel of each source. 
    * @param systemCfgs puredata basic system configuration: sample_rate, block_size
    */
    ZerrAudioDisperser(int numChannel, zerr::t_systemConfigs systemCfgs);
    /**
    * @brief initialize ZerrAudioDisperser modules
    * @return bool status of initialized or not
    */
    bool initialize();
    /**
    * @brief main callback function to perform process on buffer.
    * @param ports pointers to the in/out ports(in/out share the same memory)
    * @param n_vec actual audio vector size. Could be smaller than system block size
    */
    void perform(float **ports, int n_vec);
    /**
    * @brief return the total number of inlet plus outlet
    * @return TODO
    */
    int get_port_count(); //TODO: remove if not needed
    /**
    * @brief free a ZerrAudioDisperser instance
    */
    ~ZerrAudioDisperser();

private:
    zerr::t_blockIns  inputBuffer;  /**< multi-channel input buffer  */
    zerr::t_blockOuts outputBuffer; /**< multi-channel output buffer */
    float **inPtr;                  /**< PD-style input data pointer  */
    float **outPtr;                 /**< PD-style output data pointer */

    //module objects
    zerr::AudioDisperser      *audioDisperser;  /**< TODO */
    zerr::Logger              *logger;          /**< TODO */
};





