/**
 * @file zerr_disperser.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief AudioDisperser Class Puredata Wrapper - Provides interface between Pure Data and the core audio dispersion functionality
 * @date 2024-01-29
 *
 * @copyright Copyright (c) 2023-2024
 */
#pragma once
#include "m_pd.h"

#include "audiodisperser.h"
#include "logger.h"
#include "types.h"
#include "utils.h"

/**
 * @class ZerrDisperser
 * @brief Main wrapper class that interfaces between Pure Data and the core audio dispersion functionality
 * 
 * This class handles the initialization, audio processing, and cleanup of audio dispersion operations.
 * It manages the data flow between Pure Data's audio system and the internal audio processing chain.
 */
class ZerrDisperser {
  public:
    int numInlet;  /**< Number of signal inlets for receiving audio input */
    int numOutlet; /**< Number of signal outlets for sending processed audio output */
    /**
     * @brief Creates a new ZerrDisperser instance
     * @param numChannel The number of audio channels per source to handle
     * @param systemCfgs Pure Data system configuration containing sample rate and block size settings
     */
    ZerrDisperser(int numChannel, zerr::SystemConfigs systemCfgs);
    /**
     * @brief Initializes all internal components and prepares the object for processing
     * @return true if initialization was successful, false otherwise
     */
    bool initialize();
    /**
     * @brief Main DSP callback function that processes audio buffers
     * @param ports Array of pointers to input/output audio buffers (shared memory between in/out)
     * @param n_vec The actual size of audio vectors to process (may be smaller than system block size)
     */
    void perform(float **ports, int n_vec);
    /**
     * @brief Gets the total number of ports (inlets + outlets)
     * @return Total count of all audio ports
     */
    int get_port_count();  // TODO(Zeyu Yang): remove if not needed
    /**
     * @brief Destructor that cleans up and frees all allocated resources
     */
    ~ZerrDisperser();

  private:
    zerr::Blocks inputBuffer;  /**< Multi-channel buffer for storing incoming audio samples */
    zerr::Blocks outputBuffer; /**< Multi-channel buffer for storing processed audio samples */
    float **inPtr;            /**< Array of pointers to Pure Data input signal vectors */
    float **outPtr;           /**< Array of pointers to Pure Data output signal vectors */

    zerr::AudioDisperser *audioDisperser; /**< Core component that implements the audio dispersion algorithms */
    zerr::Logger *logger;    /**< Logging utility for debug and error messages across platforms */
};
