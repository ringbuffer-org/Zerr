/**
 * @file envelopecombinator.h 
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Envelope Combinator Class Implementation
 * @version 0.2
 * @date 2023-08-05
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef ENVELOPECOMBINATOR_H
#define ENVELOPECOMBINATOR_H

#include "utils.h"
#include "types.h"
#include "logger.h"

namespace zerr {
/**
* @brief Combine the multi-channel envelopes from different sources using selected combination mode.
*/
class EnvelopeCombinator { 
public:
    int numInlet;   /**< number of inlets  */
    int numOutlet;  /**< number of outlets */
    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    EnvelopeCombinator(int numSource, int numChannel, zerr::t_systemConfigs systemCfgs, std::string combinationMode);
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
    int get_block_size(){return systemCfgs.block_size;}
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
private:
    int                    numSource;       /**< TODO */
    int                    numChannel;      /**< TODO */
    zerr::t_systemConfigs  systemCfgs;      /**< system configuration: sample_rate, block_size */
    std::string            combinationMode; /**< TODO */

    Logger                *logger;          /**< TODO */

    t_blockIns             inputBuffer;      /**< TODO */
    t_blockOuts            outputBuffer;     /**< TODO */

    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    void _process_add();
    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    void _process_root();
    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    void _process_max();
}; 

} //namespace zerr
#endif // ENVELOPECOMBINATOR_H
