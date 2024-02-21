/**
 * @file envelopegenerator.h 
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Zerr* Envelope Generator Class Header
 *        The EnvelopeGenerator class is designed for generating audio envelopes based on input control signals
 *        and speaker array configurations. It supports various generation modes and can manipulate speaker activation
 *        and trajectory for dynamic spatial audio experiences.
 * @date 2024-02-07
 * 
 * @copyright Copyright (c) 2023-2024
 */
#ifndef CORE_ENVELOPEGENERATOR_H
#define CORE_ENVELOPEGENERATOR_H

#include "types.h"

#include "logger.h"
#include "speakermanager.h"
#include "onsetdetector.h"

namespace zerr {

/**
 * @class EnvelopeGenerator
 * @brief Generates envelopes according to input control signals and speaker array setups.
 * 
 * This class provides functionalities for envelope generation in a spatial audio context, 
 * allowing dynamic interaction with a configured speaker array. It supports multiple generation
 * modes including trigger and trajectory-based envelope shaping.
 */
class EnvelopeGenerator { 
 public:
    const int numInlet = 3;    ///< Number of inlets: main(1), spread(2), volume(3).
    int       numOutlet;       ///< Number of outlets, assigned according to the speaker configuration.
    /**
    * @brief Constructor of EnvelopeGenerator setups the parameters for initializing this Class.
    * 
    * @param systemCfgs System configuration: sample rate, block size etc.
    * @param spkrCfgFile Path to the speaker array setup configuration file to initialize the SpeakerManager 
    * @param selectionMode The strategy for generating envelope: trigger | trajectory
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
     * @brief Gets the number of active speakers in the current speaker array setup.
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
    * @brief Add/Set/Delete the active speakers
    * @param idxs 
    * @param action 
    */
    void setActiveSpeakerIndexs(std::string action, Indexes idxs);
    /**
    * @brief TODO
    * @param idxs
    * @param action
    */
    void setTrajectoryVector(Indexes idxs);
    /**
    * @brief TODO
    */
    void setTopoMatrix(std::string action, Indexes idxs);
    /**
    * @brief TODO
    */
    void setTriggerInterval(Param newInterval);
    /**
    * @brief TODO
    */
    void printParameters();
    /**
    * @brief reset all parameters and reinitialize. emtpy for now
    */
    void reset();
    /**
    * @brief TODO
    */
    ~EnvelopeGenerator();

 private:
    typedef void (EnvelopeGenerator::*ProcessFunction)();
    ProcessFunction processFunc;

    SystemConfigs systemCfgs;  /**< System configurations: sample rate, block size etc. */

    ConfigPath    speakerCfgs;     /**< Path to the speaker array setup configuration file */
    Mode          genMode;   /**< The strategy for generating envelope: trigger | trajectory */
    Mode          triggerMode;     /**< The strategy for choosing the next speaker to jump to using trigger with topology */

    AudioBuffers            inputBuffers;    /**< multi-channel input buffer in the shape of input channel number x block size */
    AudioBuffers           outputBuffers;    /**< multi-channel output buffer in the shape of output channel number x block size */

    SpeakerManager        *speakerManager; /**< SpeakerManger object to access the speaker array information */
    Logger                *logger;         /**< Logger object for printing logs to all kinds of console */
    OnsetDetector         *onsetDetector;

    std::map<Index, size_t> indexChannelLookup;  /**< index to channel reverse lookup table */
    /**
    * @brief trigger mode envelope generation process. 
    *           jump to a new output speaker when ever a trigger received
    */
    void _processTrigger();
    /**
    * @brief trajectory mode envelope generation process.
    */
    void _processTrajectory();
    /**
    * @brief setup the indexChannelLookup
    * @param the unmasked speaker indexs
    */
    // void _set_index_channel_lookup(Indexes indexs);
    /**
    * @brief calculate the spread gain according the distance between main output speaker and spread speaker
    * @param x distance
    * @param theta control parameter between 0.0-1.0 
    *           0.0 means no spread at all. 
    *           1.0 means spead to all speakers equally
    */
    Param _calculateGain(Param x, Param theta); 
}; 

} //namespace zerr
#endif // CORE_ENVELOPEGENERATOR_H