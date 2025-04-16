/**
 * @file envelopegenerator.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Zerr* Envelope Generator Class Header
 *        The EnvelopeGenerator class is designed for generating audio envelopes
 * based on input control signals and speaker array configurations. It supports
 * various generation modes and can manipulate speaker activation and trajectory
 * for dynamic spatial audio experiences.
 * @date 2024-02-07
 *
 * @copyright Copyright (c) 2023-2025
 */
#ifndef CORE_ENVELOPEGENERATOR_H
#define CORE_ENVELOPEGENERATOR_H

#include "logger.h"
#include "onsetdetector.h"
#include "speakermanager.h"
#include "types.h"

namespace zerr {

/**
 * @class EnvelopeGenerator
 * @brief Generates envelopes according to input control signals and speaker
 * array setups.
 *
 * This class provides functionalities for envelope generation in a spatial
 * audio context, allowing dynamic interaction with a configured speaker array.
 * It supports multiple generation modes including trigger and trajectory-based
 * envelope shaping.
 */
class EnvelopeGenerator {
 public:
    const int numInlet = 3; ///< Number of inlets: main(1), spread(2), volume(3).
    int numOutlet; ///< Number of outlets, assigned according to the speaker
                   ///< configuration.
    /**
     * @brief Constructor of EnvelopeGenerator setups the parameters for
     * initializing this Class.
     *
     * @param systemCfgs System configuration: sample rate, block size etc.
     * @param spkrCfgFile Path to the speaker array setup configuration file to
     * initialize the SpeakerManager
     * @param selectionMode The strategy for generating envelope: trigger |
     * trajectory
     */
    EnvelopeGenerator(SystemConfigs systemCfgs, ConfigPath speakerCfgs, Mode genMode);
    /**
     * @brief Initialize the EnvelopeGenerator.
     *
     * Initialize SpeakerManager, set numOutlet, initialize I/O buffer
     * @return bool whether correctly initialized
     */
    bool initialize();
    /**
     * @brief Main callback function of EnvelopeGenerator class.
     *
     * @param in input multi-channel audio blocks
     * @return Blocks The generated multi-channel envelopes.
     */
    Blocks perform(Blocks in);
    /**
     * @brief Gets the number of active speakers in the current speaker array
     * setup.
     *
     * @return int Number of active speakers.
     */
    int getNumSpeakers();
    /**
     * @brief Set the current output speaker. Only used in trigger mode.
     * @param newIdx the index of the next speaker
     */
    void setCurrentSpeaker(Index newIdx);
    /**
     * @brief Add/Set/Delete the active speakers in the speaker array
     * @param action The action to perform: "add", "set", or "delete"
     * @param idxs Array of speaker indices to modify
     */
    void setActiveSpeakerIndexs(std::string action, Indexes idxs);
    /**
     * @brief Sets the trajectory vector for envelope movement between speakers
     * @param idxs Array of speaker indices defining the trajectory path
     */
    void setTrajectoryVector(Indexes idxs);
    /**
     * @brief Modifies the topology matrix defining speaker connections
     * @param action The action to perform: "add", "set", or "delete"
     * @param idxs Array of indices defining topology connections
     */
    void setTopoMatrix(std::string action, Indexes idxs);
    /**
     * @brief Sets the time interval between trigger events
     * @param newInterval The new interval value in milliseconds
     */
    void setTriggerInterval(Param newInterval);
    /**
     * @brief Prints the current parameter settings to the logger
     */
    void printParameters();
    /**
     * @brief Resets all parameters to default values and reinitializes the generator
     */
    void reset();
    /**
     * @brief Destructor that cleans up allocated resources
     */
    ~EnvelopeGenerator();

    /**
     * @brief xxxxx
     */
    void setPrinter(Logger::PrintStrategy newPrinter);

    Logger* logger; /**< Logger object for printing logs to all kinds of console */

    SpeakerManager* speakerManager; /**< SpeakerManger object to access the speaker array information */
 private:
    typedef void (EnvelopeGenerator::*ProcessFunction)();
    ProcessFunction processFunc;

    SystemConfigs systemCfgs; /**< System configurations: sample rate, block size etc. */

    ConfigPath speakerCfgs; /**< Path to the speaker array setup configuration file */
    Mode genMode; /**< The strategy for generating envelope: trigger |
                     trajectory */
    Mode triggerMode; /**< The strategy for choosing the next speaker to jump to
                         using trigger with topology */

    AudioBuffers inputBuffers; /**< multi-channel input buffer in the shape of
                                  input channel number x block size */
    AudioBuffers outputBuffers; /**< multi-channel output buffer in the shape of
                                   output channel number x block size */

    // SpeakerManager* speakerManager; /**< SpeakerManger object to access the speaker array information */

    OnsetDetector* onsetDetector; /**< Detector for identifying onset triggers in the input signal */

    std::map<Index, size_t> indexChannelLookup; /**< index to channel reverse lookup table */
    /**
     * @brief trigger mode envelope generation process.
     *           jump to a new output speaker when ever a trigger received
     */
    void _processTrigger();
    /**
     * @brief trajectory mode envelope generation process.
     *        generates envelopes following the defined trajectory path
     */
    void _processTrajectory();
    /**
     * @brief setup the indexChannelLookup
     * @param the unmasked speaker indexs
     */
    // void _set_index_channel_lookup(Indexes indexs);
    /**
     * @brief calculate the spread gain according the distance between main
     * output speaker and spread speaker
     * @param x distance between speakers (normalized 0.0-1.0)
     * @param theta control parameter between 0.0-1.0
     *           0.0 means no spread at all.
     *           1.0 means spead to all speakers equally
     * @return Param The calculated gain value between 0.0-1.0
     */
    Param _calculateGain(Param x, Param theta);
};

} // namespace zerr
#endif // CORE_ENVELOPEGENERATOR_H