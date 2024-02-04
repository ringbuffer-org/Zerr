/**
 * @file zerr_envelopes.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief EnvelopeGenerator Class Puredata Wrapper
 * @version 0.4
 * @date 2023-08-04
 * 
 * @copyright Copyright (c) 2023
 */
#pragma once

#include <stdlib.h>
#include <string>

#include "types.h"
#include "utils.h"
#include "ringbuffer.h"
#include "logger.h"

#include "envelopegenerator.h"

class ZerrEnvelopes{
 public:
    const int numInlet = 3;  /**< number of inlets: main(1)/spread(2)/volume(3) */
    int numOutlet;           /**< number of outlets: assgined according to the speaker configuration*/
    /**
    * @brief create a new zerr_envelopes instance 
    * @param systemCfgs puredata basic system configuration: sample_rate, block_size
    * @param selectionMode the corresponding mode of the envelope generator to the control signal: trigger & trajectory
    * @param spkrCfgFile path of the speaker array configuration
    */
    ZerrEnvelopes(zerr::t_systemConfigs systemCfgs, std::string selectionMode, std::string spkrCfgFile);
    /**
    * @brief initialize zerr_envelopes modules
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
    * @return number of inlets pluse number of outlets
    */
    int get_port_count();
    /**
    * @brief TODO
    */
    void setActiveSpeakerIndexs(char* action, int* idxs, size_t size);
    /**
    * @brief set the trajectory list
    * @param idxs
    */
    void setTrajectoryVector(int* idxs, size_t size);
    /**
    * @brief print current statue
    */
    void print_parameters(char* name);
    /**
    * @brief free a zerr_envelopes instance
    */
    ~ZerrEnvelopes();

 private:
    zerr::t_systemConfigs systemCfgs;  /**< Pure data system configurations */

    zerr::BlockIns  inputBuffer;  /**< Zerr input buffer, C++ style */
    zerr::t_blockOuts outputBuffer; /**< Zerr output buffer, C++ style */
    float **inPtr;                  /**< PD input pointer, C style */
    float **outPtr;                 /**< PD output pointer, C style */

    std::string spkrCfgFile;   /**< speaker configuration */
    std::string selectionMode; /**< the mode for selecting output speakers */

    zerr::EnvelopeGenerator  *envelopeGenerator; /**< pointer to the Zerr EnvelopeGenerator object */
    zerr::Logger             *logger;            /**< pointer to the Zerr Logger object */
};





