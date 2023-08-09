/**
 * @file envelopegenerator.h 
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Envelope Generator Class Implementation
 * @version 0.5
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef ENVELOPEGENERATOR_H
#define ENVELOPEGENERATOR_H

#include "utils.h"
#include "types.h"
#include "logger.h"

#include "speakermanager.h"

namespace zerr {
/**
* @brief EnvelopeGenerator generates evelope stream according to input control signal and the speaker array setups
*/
class EnvelopeGenerator { 
public:
    const int numInlet = 3; /**< number of inlets: main(1)/spread(2)/valume(3) */
    int numOutlet;          /**< number of outlets: assgined according to the speaker configuration*/
    /**
    * @brief Constructor of EnvelopeGenerator setups the parameters for initializing this Class.
    * @param systemCfgs System configuration: sample rate, block size etc.
    * @param spkrCfgFile Path to the speaker array setup configuration file to initialize the SpeakerManager 
    * @param selectionMode The strategy for generating envelope: trigger | trajectory
    */
    EnvelopeGenerator(t_systemConfigs systemCfgs, std::string spkrCfgFile, std::string selectionMode);
    /**
    * @brief initialize the EnvelopeGenerator. initialize SpeakerManager, set numOutlet, initialize I/O buffer
    * @return bool whether correctly initialized
    */
    bool initialize();
    /**
    * @brief setup the inputBuffer with new audio block
    * @param in the audio block to be processed
    */
    void fetch(t_blockIns in);
    /**
    * @brief generate the envelope signal block to outputBuffer
    */
    void process();
    /**
    * @brief return the audio block in outputBuffer 
    * @return t_blockOuts
    */
    t_blockOuts send();
    /**
    * @brief get the number of active speaker of current speaker array setup
    * @return int number of active speakerss
    */
    int get_n_speaker();
    /**
    * @brief set the current output speaker 
    * @param newIdx the index of next speaker
    */
    void set_current_speaker(t_index newIdx);
    /**
    * @brief TODO 
    * @param idxs
    * @param action
    */
    void manage_unmasked_indexs(std::string action, t_indexs idxs);
    /**
    * @brief TODO
    */
    void print_parameters(std::string name);
    /**
    * @brief reset all parameters and reinitialize. emtpy for now
    */
    void reset();

private:
    t_index               currIdx;         /**< The index of current output speaker */

    t_systemConfigs       systemCfgs;      /**< System configuration structure: sample rate, block size etc. */

    std::string           spkrCfgFile;     /**< Path to the speaker array setup configuration file */
    std::string           selectionMode;   /**< The strategy for generating envelope: trigger | trajectory */
    int                   triggerMode;     /**< The strategy for choosing the next speaker to jump to using trigger with topology */

    t_blockIns            inputBuffer;     /**< multi-channel input buffer in the shape of input channel number x block size */
    t_blockOuts           outputBuffer;    /**< multi-channel output buffer in the shape of output channel number x block size */

    SpeakerManager        *speakerManager; /**< SpeakerManger object to access the speaker array information */
    Logger                *logger;         /**< Logger object for printing logs to all kinds of console */

    std::map<t_index, int> indexChannelLookup;  /**< map between speaker index and channal index for N(1) access */
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
    void _set_index_channel_lookup(t_indexs indexs);
    /**
    * @brief This function is obsolete. check _calculate_gain() instead
    */
    t_value _calculate_normal_distribution(t_value x, t_value alpha);
    /**
    * @brief calculate the spread gain according the distance between main output speaker and spread speaker
    * @param x distance
    * @param theta control parameter between 0.0-1.0 
    *           0.0 means no spread at all. 
    *           1.0 means spead to all speakers equally
    */
    t_value _calculate_gain(t_value x, t_value theta); 
}; 

} //namespace zerr
#endif // ENVELOPEGENERATOR_H