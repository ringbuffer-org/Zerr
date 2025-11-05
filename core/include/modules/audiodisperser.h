/**
 * @file audiodisperser.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief AudioDisperser Class Header
 * @date 2024-02-06
 *
 * @copyright Copyright (c) 2023-2024
 */
#ifndef AUDIODISPERSER_H
#define AUDIODISPERSER_H

#include "logger.h"
#include "types.h"

namespace zerr {
/**
 * @brief Class for dispersing audio signals across multiple channels
 */
class AudioDisperser {
  public:
    int numInlet;  /**< number of inlets  */
    int numOutlet; /**< number of outlets */
    /**
     * @brief Construct a new Audio Disperser object
     * @param numChannel Number of audio channels to disperse across
     * @param systemCfgs System configuration containing sample rate and block
     * size
     */
    AudioDisperser(int numChannel, zerr::SystemConfigs systemCfgs);
    /**
     * @brief Initialize the audio disperser
     * @return true if initialization successful, false otherwise
     */
    bool initialize();
    /**
     * @brief Process input audio blocks and disperse across channels
     * @param in Input audio blocks to process
     * @return Processed and dispersed output audio blocks
     */
    Blocks perfrom(Blocks in);
    /**
     * @brief Get the block size used by the audio disperser
     * @return The block size from system configuration
     */
    int get_block_size() { return systemCfgs.block_size; }

  private:
    int numChannel;              /**< Number of audio channels for dispersal */
    SystemConfigs systemCfgs;    /**< system configuration: sample_rate, block_size */
    std::string combinationMode; /**< Mode for combining audio signals */
    Logger* logger;              /**< Logger instance for debug/error messages */
    Blocks inputBuffer;          /**< Buffer for storing input audio blocks */
    Blocks outputBuffer;         /**< Buffer for storing processed output blocks */
};

} // namespace zerr
#endif // AUDIODISPERSER_H
