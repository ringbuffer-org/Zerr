/**
 * @file envelopegenerator.h 
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Zerr* Envelope Generator Class Header
 * @date 2024-02-07
 * 
 * @copyright Copyright (c) 2023-2024
 */
#ifndef CORE_ENVELOPEGENERATOR_H
#define CORE_ENVELOPEGENERATOR_H

// #include "utils.h"
#include "types.h"

#include "logger.h"
#include "speakermanager.h"

namespace zerr {
/**
* @brief EnvelopeGenerator generates evelopes according to the input control signals
*  and the speaker array setups. 
*/
class EnvelopeGenerator { 
 public:
    const int numInlet = 3;    /**< number of inlets: main(1)/spread(2)/valume(3) */
    int       numOutlet;       /**< number of outlets: assgined according to the speaker configuration*/
    /**
    * @brief Constructor of EnvelopeGenerator setups the parameters for initializing this Class.
    * @param systemCfgs System configuration: sample rate, block size etc.
    * @param spkrCfgFile Path to the speaker array setup configuration file to initialize the SpeakerManager 
    * @param selectionMode The strategy for generating envelope: trigger | trajectory
    */
    EnvelopeGenerator(SystemConfigs systemCfgs, ConfigPath speakerCfgs, Mode genMode);
    /**
    * @brief Initialize the EnvelopeGenerator. initialize SpeakerManager, set numOutlet, initialize I/O buffer
    * @return bool whether correctly initialized
    */
    bool initialize();
    /**
    * @brief Main callback function of EnvelopeGenerator class.  
    * @param in input multi-channel audio blocks
    * @return the generated multi-channel envelopes
    */
    Blocks perform(Blocks in);
    /**
    * @brief Get the number of active speaker of current speaker array setup
    * @return int number of active speakers
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

    // Index currIdx;  /**< The index of current output speaker. This is only used for trigger mode. */
    
    // Samples firstSpkrGains;
    // Samples secondSpkrGains;

    SystemConfigs systemCfgs;  /**< System configurations: sample rate, block size etc. */

    ConfigPath    speakerCfgs;     /**< Path to the speaker array setup configuration file */
    Mode          genMode;   /**< The strategy for generating envelope: trigger | trajectory */
    Mode          triggerMode;     /**< The strategy for choosing the next speaker to jump to using trigger with topology */

    AudioBuffers            inputBuffers;    /**< multi-channel input buffer in the shape of input channel number x block size */
    AudioBuffers           outputBuffers;    /**< multi-channel output buffer in the shape of output channel number x block size */

    SpeakerManager        *speakerManager; /**< SpeakerManger object to access the speaker array information */
    Logger                *logger;         /**< Logger object for printing logs to all kinds of console */

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