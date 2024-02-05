/**
 * @file zerr_combinator.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief EnvelopeCombinator Class Puredata Wrapper
 * @date 2024-01-29
 * 
 * @copyright Copyright (c) 2023-2024
 */
#pragma once

#include <string>

#include "types.h"
// #include "utils.h"
#include "logger.h"

#include "./envelopecombinator.h"

class ZerrCombinator{
 public:
    int numInlet;     /**< number of inlets */
    int numOutlet;    /**< number of outlets */
    /**
    * @brief create a new ZerrCombinator instance 
    * @param numSource the number of intput multi-channel envelope source
    * @param numChannel the number of channel of each source. 
    * @param systemCfgs puredata basic system configuration: sample_rate, block_size
    * @param combinationMode select the mode about how to combine the envelopes
    */
    ZerrCombinator(int numSource, int numChannel,
        std::string combinationMode, zerr::SystemConfigs systemCfgs);
    /**
    * @brief initialize ZerrCombinator modules
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
    * @return int inlet and outlet number
    */
    int get_port_count();  // TODO(Zeyu Yang): remove if not needed
    /**
    * @brief free the ZerrCombinator instance
    */
    ~ZerrCombinator();

 private:
    zerr::Blocks  inputBuffer;  /**< multi-channel input buffer  */
    zerr::Blocks outputBuffer; /**< multi-channel output buffer */
    float **inPtr;                  /**< PD-style input data pointer  */
    float **outPtr;                 /**< PD-style output data pointer */

    zerr::EnvelopeCombinator  *envelopeCombinator;  /**< Core AudioDisperser component */
    zerr::Logger              *logger;              /**< Zerr logger for cross-platform logging*/
};





