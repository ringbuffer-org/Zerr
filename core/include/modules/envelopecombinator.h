/**
 * @file envelopecombinator.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Envelope Combinator Class Implementation
 * @date 2024-02-03
 *
 * @copyright Copyright (c) 2023-2024
 */
#ifndef ENVELOPECOMBINATOR_H
#define ENVELOPECOMBINATOR_H

#include "logger.h"
#include "types.h"
#include "utils.h"

namespace zerr {
/**
 * @brief Combine the multi-channel envelopes from different sources using
 * selected combination mode.
 */
class EnvelopeCombinator {
  public:
    int numInlet;  /**< number of inlets  */
    int numOutlet; /**< number of outlets */
    /**
     * @brief TODO
     * @param TODO
     * @param TODO
     * @param TODO
     */
    EnvelopeCombinator(int numSource, int numChannel,
                       zerr::SystemConfigs systemCfgs,
                       std::string combinationMode);
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
    // void fetch(Blocks in);
    /**
     * @brief TODO
     * @param TODO
     * @param TODO
     * @param TODO
     */
    int get_block_size() { return systemCfgs.block_size; }
    /**
     * @brief TODO
     * @param TODO
     * @param TODO
     * @param TODO
     */
    // void process();
    /**
     * @brief TODO
     * @param TODO
     * @param TODO
     * @param TODO
     */
    // Blocks send();
    /**
     * @brief TODO
     * @param TODO
     * @param TODO
     * @param TODO
     */
    Blocks perform(Blocks in);
    /**
     * @brief TODO
     * @param TODO
     * @param TODO
     * @param TODO
     */
    ~EnvelopeCombinator();

  private:
    typedef void (EnvelopeCombinator::*ProcessFunction)();
    ProcessFunction processFunc;

    int numSource;  /**< TODO */
    int numChannel; /**< TODO */
    zerr::SystemConfigs
        systemCfgs;       /**< system configuration: sample_rate, block_size */
    std::string combMode; /**< TODO */

    Logger *logger; /**< TODO */

    Blocks inputBuffer;  /**< TODO */
    Blocks outputBuffer; /**< TODO */

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

}  // namespace zerr
#endif  // ENVELOPECOMBINATOR_H
