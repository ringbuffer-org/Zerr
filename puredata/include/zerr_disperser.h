/**
 * @file zerr_disperser.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief AudioDisperser Class Puredata Wrapper
 * @date 2024-01-29
 *
 * @copyright Copyright (c) 2023-2024
 */
#pragma once

#include "audiodisperser.h"
#include "logger.h"
#include "types.h"
#include "utils.h"

class ZerrDisperser {
  public:
    int numInlet;  /**< number of inlets*/
    int numOutlet; /**< number of outlets*/
    /**
     * @brief create a new ZerrDisperser instance
     * @param numChannel the number of channel of each source.
     * @param systemCfgs puredata basic system configuration: sample_rate,
     * block_size
     */
    ZerrDisperser(int numChannel, zerr::SystemConfigs systemCfgs);
    /**
     * @brief initialize ZerrDisperser modules
     * @return bool status of initialized or not
     */
    bool initialize();
    /**
     * @brief main callback function to perform process on buffer.
     * @param ports pointers to the in/out ports(in/out share the same memory)
     * @param n_vec actual audio vector size. Could be smaller than system block
     * size
     */
    void perform(float **ports, int n_vec);
    /**
     * @brief return the total number of inlet plus outlet
     * @return int inlet and outlet number
     */
    int get_port_count();  // TODO(Zeyu Yang): remove if not needed
    /**
     * @brief free the ZerrDisperser instance
     */
    ~ZerrDisperser();

  private:
    zerr::Blocks inputBuffer;  /**< multi-channel input buffer  */
    zerr::Blocks outputBuffer; /**< multi-channel output buffer */
    float **inPtr;             /**< PD-style input data pointer  */
    float **outPtr;            /**< PD-style output data pointer */

    zerr::AudioDisperser *audioDisperser; /**< Core AudioDisperser component */
    zerr::Logger *logger; /**< Zerr logger for cross-platform logging*/
};
