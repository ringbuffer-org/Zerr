/**
 * @file audiodisperser.h 
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief AudioDisperser Class Implementation
 * @version 0.2
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef AUDIODISPERSER_H
#define AUDIODISPERSER_H

// #include "utils.h"
#include "types.h"
#include "logger.h"

namespace zerr {
/**
* @brief TODO
*/
class AudioDisperser { 
public:
    int numInlet;   /**< number of inlets  */
    int numOutlet;  /**< number of outlets */
    /**
    * @brief TODO
    * @param TODO
    * @param TODO
    * @param TODO
    */
    AudioDisperser(int numChannel, zerr::SystemConfigs systemCfgs);
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
    void fetch(Blocks in);
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
    Blocks send();
private:
    int                    numChannel;      /**< TODO */
    zerr::SystemConfigs  systemCfgs;      /**< system configuration: sample_rate, block_size */
    std::string            combinationMode; /**< TODO */

    Logger                *logger;          /**< TODO */

    Blocks             inputBuffer;      /**< TODO */
    Blocks            outputBuffer;     /**< TODO */
}; 

} //namespace zerr
#endif // AUDIODISPERSER_H