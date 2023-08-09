/**
 * @file zerr_envelope_generator.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief EnvelopeGenerator Class Puredata Wrapper
 * @version 0.4
 * @date 2023-08-04
 * 
 * @copyright Copyright (c) 2023
 */
#pragma once
// standard 
#include <stdlib.h>
// utils
#include "types.h"
#include "utils.h"
#include "ringbuffer.h"
#include "logger.h"
// modules
#include "envelopegenerator.h"

class ZerrEnvelopeGenerator{
public:
    const int numInlet = 3; /**< number of inlets: main(1)/spread(2)/volume(3) */
    int numOutlet;          /**< number of outlets: assgined according to the speaker configuration*/
    /**
    * @brief create a new zerr_envelope_generator instance 
    * @param systemCfgs puredata basic system configuration: sample_rate, block_size
    * @param selectionMode the corresponding mode of the envelope generator to the control signal: trigger & trajectory
    * @param spkrCfgFile path of the speaker array configuration
    */
    ZerrEnvelopeGenerator(zerr::t_systemConfigs systemCfgs, std::string selectionMode, std::string spkrCfgFile);
    /**
    * @brief initialize zerr_envelope_generator modules
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
    int get_port_count();
    /**
    * @brief TODO
    */
    void manage_unmasked_indexs(char* action, int* idxs, size_t size);
    /**
    * @brief TODO
    */
    void print_parameters(char* name);
    /**
    * @brief free a zerr_envelope_generator instance
    */
    ~ZerrEnvelopeGenerator();

private:
    zerr::t_systemConfigs systemCfgs;  /**< TODO */

    zerr::t_blockIns  inputBuffer;  /**< TODO */
    zerr::t_blockOuts outputBuffer; /**< TODO */
    float **inPtr;                  /**< TODO */
    float **outPtr;                 /**< TODO */

    std::string spkrCfgFile;   /**< TODO */
    std::string selectionMode; /**< TODO */

    //module objects
    zerr::EnvelopeGenerator  *envelopeGenerator; /**< TODO */
    zerr::Logger             *logger;            /**< TODO */
};





