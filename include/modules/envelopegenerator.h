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

// #include "utils.h"
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
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    EnvelopeGenerator(t_systemConfigs systemCfgs, std::string spkrCfgFile, std::string selectionMode);
    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    bool initialize();
    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    void fetch(t_blockIns in);
    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    int get_n_speaker();
    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    void set_current_speaker(t_index newIdx);
    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    void process();
    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    t_blockOuts send();





    void reset();

private:
    t_index               currIdx;         /**< TODO */

    t_systemConfigs       systemCfgs;      /**< TODO */

    std::string           spkrCfgFile;     /**< TODO */
    std::string           selectionMode;   /**< TODO */
    int                   triggerMode;     /**< TODO */

    t_blockIns            inputBuffer;     /**< TODO */
    t_blockOuts           outputBuffer;    /**< TODO */

    SpeakerManager        *speakerManager; /**< TODO */
    Logger                *logger;         /**< TODO */

    std::map<t_index, int> indexChannelLookup;  /**< TODO */
    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    void _process_trigger();
    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    void _process_trajectory();
    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    void _set_index_channel_lookup(t_indexs indexs);
    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    t_value _calculate_normal_distribution(t_value x, t_value alpha);
    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    t_value _calculate_gain(t_value x, t_value theta); 
}; 

} //namespace zerr
#endif // ENVELOPEGENERATOR_H