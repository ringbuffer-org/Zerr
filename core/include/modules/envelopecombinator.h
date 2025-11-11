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

#include <functional>
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
     * @brief Construct a new Envelope Combinator object
     * @param numSource Number of envelope sources to combine
     * @param numChannel Number of channels per source
     * @param systemCfgs System configuration containing sample rate and block size
     * @param combinationMode Mode for combining envelopes ("add", "root", or "max")
     */
    EnvelopeCombinator(int numSource, int numChannel, zerr::SystemConfigs systemCfgs,
                       std::string combinationMode);
    /**
     * @brief Initialize the envelope combinator
     * @return true if initialization successful, false otherwise
     */
    bool initialize();
    /**
     * @brief Get the block size used by the envelope combinator
     * @return The block size from system configuration
     */
    int get_block_size() { return systemCfgs.block_size; }
    /**
     * @brief Process input envelope blocks and combine using selected mode
     * @param in Input envelope blocks to process
     * @return Combined output envelope blocks
     */
    Blocks perform(Blocks in);
    /**
     * @brief Destructor for the Envelope Combinator
     */
    ~EnvelopeCombinator();

  private:
    typedef void (EnvelopeCombinator::*ProcessFunction)();
    ProcessFunction processFunc;

    int numSource;                  /**< Number of envelope sources to combine */
    int numChannel;                 /**< Number of channels per source */
    zerr::SystemConfigs systemCfgs; /**< system configuration: sample_rate, block_size */
    std::string combMode;           /**< Mode for combining envelopes */

    Logger* logger; /**< Logger instance for debug/error messages */

    Blocks inputBuffer;  /**< Buffer for storing input envelope blocks */
    Blocks outputBuffer; /**< Buffer for storing combined output blocks */

    /**
     * @brief Process envelopes using addition combination mode
     * Adds envelope values across sources for each channel
     */
    void _process_add();
    /**
     * @brief Process envelopes using root mean square combination mode
     * Combines envelopes using RMS calculation across sources
     */
    void _process_root();
    /**
     * @brief Process envelopes using maximum combination mode
     * Takes maximum envelope value across sources for each channel
     */
    void _process_max();
};

} // namespace zerr
#endif // ENVELOPECOMBINATOR_H
