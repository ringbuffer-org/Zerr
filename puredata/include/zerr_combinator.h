/**
 * @file zerr_combinator.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief EnvelopeCombinator Class Puredata Wrapper - Provides interface between Pure Data and the core envelope combination functionality
 * @date 2024-01-29
 *
 * @copyright Copyright (c) 2023-2024
 */
#pragma once
#include "m_pd.h"

#include <string>

#include "types.h"
// #include "utils.h"
#include "./envelopecombinator.h"
#include "logger.h"

/**
 * @class ZerrCombinator
 * @brief Main wrapper class that interfaces between Pure Data and the core envelope combination functionality
 *
 * This class handles the initialization, audio processing, and cleanup of envelope combination operations.
 * It manages the data flow between Pure Data's audio system and the internal envelope processing chain.
 */
class ZerrCombinator {
 public:
    int numInlet; /**< Number of signal inlets for receiving audio input */
    int numOutlet; /**< Number of signal outlets for sending processed audio output */
    /**
     * @brief Creates a new ZerrCombinator instance
     * @param numSource The number of input multi-channel envelope sources to process
     * @param numChannel The number of channels per source to handle
     * @param combinationMode The algorithm/mode to use for combining the envelopes (e.g. "multiply", "add", etc.)
     * @param systemCfgs Pure Data system configuration containing sample rate and block size settings
     */
    ZerrCombinator(int numSource, int numChannel, std::string combinationMode,
        zerr::SystemConfigs systemCfgs);
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
    int get_port_count(); // TODO(Zeyu Yang): remove if not needed
    /**
     * @brief Destructor that cleans up and frees all allocated resources
     */
    ~ZerrCombinator();

 private:
    zerr::Blocks inputBuffer; /**< Multi-channel buffer for storing incoming audio samples */
    zerr::Blocks outputBuffer; /**< Multi-channel buffer for storing processed audio samples */
    float** inPtr; /**< Array of pointers to Pure Data input signal vectors */
    float** outPtr; /**< Array of pointers to Pure Data output signal vectors */

    zerr::EnvelopeCombinator* envelopeCombinator; /**< Core component that implements the envelope combination algorithms */
    zerr::Logger* logger; /**< Logging utility for debug and error messages across platforms */
};
