/**
 * @file zerr_envelopes.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief EnvelopeGenerator Class Puredata Wrapper - Provides interface between Pure Data and the core envelope generation functionality
 * @date 2023-08-04
 *
 * @copyright Copyright (c) 2023-2024
 */
#pragma once

// #include <stdlib.h>
// #include <string>

#include "types.h"
// #include "utils.h"
#include "envelopegenerator.h"
#include "logger.h"
#include "ringbuffer.h"

/**
 * @class ZerrEnvelopes
 * @brief Main wrapper class that interfaces between Pure Data and the core envelope generation functionality
 * 
 * This class handles the initialization, audio processing, and cleanup of envelope generation operations.
 * It manages the data flow between Pure Data's audio system and the internal envelope processing chain.
 */
class ZerrEnvelopes {
  public:
    const int numInlet =
        3;         /**< Number of signal inlets: main(1)/spread(2)/volume(3) */
    int numOutlet; /**< Number of signal outlets based on speaker configuration */
    /**
     * @brief Creates a new ZerrEnvelopes instance
     * @param systemCfgs Pure Data system configuration containing sample rate and block size settings
     * @param selectionMode The envelope generator mode for control signal handling (trigger & trajectory)
     * @param spkrCfgFile Path to the speaker array configuration file
     */
    ZerrEnvelopes(zerr::SystemConfigs systemCfgs, std::string selectionMode,
                  std::string spkrCfgFile);
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
    void perform(float** ports, int n_vec);
    /**
     * @brief Gets the total number of ports (inlets + outlets)
     * @return Total count of all audio ports
     */
    int get_port_count();
    /**
     * @brief Updates the list of active speakers in the envelope system
     * @param action Action to perform on the speaker list ("add", "remove", etc.)
     * @param idxs Array of speaker indices to modify
     * @param size Number of indices in the array
     */
    void setActiveSpeakerIndexs(char* action, int* idxs, size_t size);
    /**
     * @brief Sets the current active speaker for envelope generation
     * @param idx Index of the speaker to set as current
     */
    void setCurrentSpeaker(int idx);
    /**
     * @brief Updates the topology matrix defining speaker relationships
     * @param action Action to perform on the topology ("set", "clear", etc.)
     * @param idxs Array of matrix values to update
     * @param size Number of values in the array
     */
    void setTopoMatrix(char* action, int* idxs, size_t size);
    /**
     * @brief Sets the trajectory path for envelope movement
     * @param idxs Array of speaker indices defining the trajectory
     * @param size Number of points in the trajectory
     */
    void setTrajectoryVector(int* idxs, size_t size);
    /**
     * @brief Sets the time interval between envelope triggers
     * @param interval Time in milliseconds between triggers
     */
    void setTriggerInterval(float interval);
    /**
     * @brief Outputs the current state information to the Pure Data console
     */
    void printParameters();
    /**
     * @brief Destructor that cleans up and frees all allocated resources
     */
    ~ZerrEnvelopes();

  private:
    zerr::SystemConfigs systemCfgs; /**< Pure Data system configuration settings */

    zerr::Blocks inputBuffer;  /**< Multi-channel buffer for storing incoming audio samples */
    zerr::Blocks outputBuffer; /**< Multi-channel buffer for storing processed audio samples */
    float** inPtr;            /**< Array of pointers to Pure Data input signal vectors */
    float** outPtr;           /**< Array of pointers to Pure Data output signal vectors */

    std::string spkrCfgFile;   /**< Path to speaker configuration file */
    std::string selectionMode; /**< Mode for selecting output speaker routing */

    zerr::EnvelopeGenerator*
        envelopeGenerator; /**< Core component that implements the envelope generation algorithms */
    zerr::Logger* logger;  /**< Logging utility for debug and error messages across platforms */
};
