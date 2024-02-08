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

#include "utils.h"
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
    EnvelopeGenerator(SystemConfigs systemCfgs, std::string spkrCfgFile, std::string selectionMode);
    /**
    * @brief initialize the EnvelopeGenerator. initialize SpeakerManager, set numOutlet, initialize I/O buffer
    * @return bool whether correctly initialized
    */
    bool initialize();
    /**
    * @brief TODO 
    * @param TODO
    * @param TODO
    */
    Blocks perform(Blocks in);
    /**
    * @brief get the number of active speaker of current speaker array setup
    * @return int number of active speakerss
    */
    int getNumSpeakers();
    /**
    * @brief set the current output speaker 
    * @param newIdx the index of next speaker
    */
    void setCurrSpeaker(Index newIdx);
    /**
    * @brief TODO 
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

private:
    Index               currIdx;         /**< The index of current output speaker */

    SystemConfigs       systemCfgs;      /**< System configuration structure: sample rate, block size etc. */

    std::string           spkrCfgFile;     /**< Path to the speaker array setup configuration file */
    std::string           selectionMode;   /**< The strategy for generating envelope: trigger | trajectory */
    int                   triggerMode;     /**< The strategy for choosing the next speaker to jump to using trigger with topology */

    Blocks            inputBuffer;     /**< multi-channel input buffer in the shape of input channel number x block size */
    Blocks           outputBuffer;    /**< multi-channel output buffer in the shape of output channel number x block size */

    SpeakerManager        *speakerManager; /**< SpeakerManger object to access the speaker array information */
    Logger                *logger;         /**< Logger object for printing logs to all kinds of console */

    std::map<Index, int> indexChannelLookup;  /**< map between speaker index and channal index for N(1) access */
    /**
    * @brief trigger mode envelope generation process. 
    *           jump to a new output speaker when ever a trigger sample received (sample value:1.0)
    */
    void _process_trigger();
    /**
    * @brief trajectory mode envelope generation process.
    */
    void _process_trajectory();
    /**
    * @brief setup the indexChannelLookup
    * @param the unmasked speaker indexs
    */
    void _set_index_channel_lookup(Indexes indexs);
    /**
    * @brief calculate the spread gain according the distance between main output speaker and spread speaker
    * @param x distance
    * @param theta control parameter between 0.0-1.0 
    *           0.0 means no spread at all. 
    *           1.0 means spead to all speakers equally
    */
    Param calculateGain(Param x, Param theta); 
}; 

} //namespace zerr
#endif // CORE_ENVELOPEGENERATOR_H