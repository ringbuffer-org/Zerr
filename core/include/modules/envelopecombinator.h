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
#include <atomic>

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
    int get_block_size() const noexcept { return systemCfgs.block_size; }
    /**
     * @brief Process input envelope blocks and combine using selected mode
     * @param in Input envelope blocks to process
     * @return Reference to the internal output buffer, valid until the next perform()
     *
     * Taken and returned by reference so that nothing is allocated on the audio thread.
     */
    const Blocks& perform(const Blocks& in);
    /**
     * @brief Change the combination mode
     * @param mode Mode name: "add", "root" or "max"
     * @return false, leaving the mode unchanged, if the name is not a known mode
     *
     * Safe to call from a control thread while perform() runs on an audio thread: the
     * mode is a lock-free atomic that perform() loads once per block, so a block is
     * never processed partly in one mode and partly in another.
     */
    bool set_mode(const std::string& mode) noexcept;
    /**
     * @brief Change the combination mode
     * @param mode The mode to switch to
     */
    void set_mode(CombMode mode) noexcept;
    /**
     * @brief Get the combination mode currently in use
     */
    [[nodiscard]] CombMode get_mode() const noexcept
    {
        return combMode.load(std::memory_order_relaxed);
    }
    /**
     * @brief Get the name of the combination mode currently in use
     * @return "add", "root" or "max"
     */
    [[nodiscard]] const char* get_mode_name() const noexcept { return toString(get_mode()); }
    ~EnvelopeCombinator() = default;

  private:
    int numSource;                  /**< Number of envelope sources to combine */
    int numChannel;                 /**< Number of channels per source */
    zerr::SystemConfigs systemCfgs; /**< system configuration: sample_rate, block_size */

    /**< Mode for combining envelopes. Atomic because a control thread may change it while
         the audio thread is inside perform(). */
    std::atomic<CombMode> combMode{CombMode::Max};
    static_assert(std::atomic<CombMode>::is_always_lock_free,
                  "CombMode must be lock-free so set_mode() never blocks the audio thread");

    bool modeValid{true}; /**< False when the constructor was given an unknown mode name */
    /**< The mode name as given to the constructor, kept only so initialize() can name it in
         its error message. Control thread only -- never read while perform() runs. */
    std::string requestedModeName;
    bool prepared{false}; /**< True once initialize() has sized the buffers */

    Logger logger; /**< Logger instance for debug/error messages */

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
