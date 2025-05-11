/**
 * @file    zerr.combinator.hpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   mc.zerr.combinator~ Max/MSP External using Max API for better multi-channel support
 * @date    2025-05-03
 *
 * @copyright  Copyright (c) 2023-2025
 * @license    MIT license
 */
#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "./envelopecombinator.h"
#include "types.h"

/**
 * @class ZerrFeatures
 * @brief Main wrapper class that interfaces between Max/MSP and the core audio feature extraction functionality
 *
 * This class handles the initialization, audio processing, and cleanup of audio feature extraction operations.
 * It manages the data flow between Max/MSP's audio system and the internal feature processing chain.
 */
class ZerrCombinator {
 public:
    /**
     * @brief Creates a new ZerrCombinator instance
     * @param sys_config System configuration containing sample rate and block size settings
     * @param ft_names List of audio features to extract from the input signal
     */
    ZerrCombinator(const zerr::SystemConfigs& sys_config, int inputCount, std::string mode)
        : systemConfigs { sys_config }
        , inputBuffer(inputCount, std::vector<double>(sys_config.block_size, 0.0))
    // , combinator { std::make_unique<zerr::EnvelopeCombinator>(sys_config, spkrCfgFile, selectionMode) }
    {
    }

    // Disable copying but allow moving
    ZerrCombinator(const ZerrCombinator&) = delete;
    ZerrCombinator& operator=(const ZerrCombinator&) = delete;
    ZerrCombinator(ZerrCombinator&&) = default;
    ZerrCombinator& operator=(ZerrCombinator&&) = default;

    /**
     * @brief Initializes all internal components and prepares the object for processing
     * @return true if initialization was successful, false otherwise
     * @throws std::runtime_error if memory allocation fails
     */
    bool initialize()
    {
        // Initialize the Envelope Generator module
        if (!combinator->initialize()) {
            post("ZerrCombinator::initialize failed to initialize combinator");
            return false;
        };

        // assgin Max/MSP print method the the logger function
        // Because we want to see logs in the Max/MSP log window
        // auto printFunc = [](const std::string& msg) {
        //     post(msg.c_str());
        // };
        // combinator->setPrinter(printFunc);

        // outputCount = combinator->getNumSpeakers();
        // post("ZerrCombinator::initialize outputCount is %d", outputCount);

        inputBuffer.resize(inputCount, zerr::Samples(systemConfigs.block_size, 0.0f));
        outputBuffer.resize(outputCount, zerr::Samples(systemConfigs.block_size, 0.0f));

        return true;
    }

    /**
     * @brief Main DSP callback function that processes audio buffers
     * @param ins Array of pointers to input audio buffers
     * @param numins Number of input channels
     * @param outs Array of pointers to output audio buffers
     * @param numouts Number of output channels
     * @param sampleframes Number of samples to process
     */
    void perform(double** ins, long numins, double** outs, long numouts, long sampleframes)
    {
        if (!ins || !outs || numins < inputCount || numouts < outputCount) {
            throw std::invalid_argument("Invalid buffer pointers or sizes in perform()");
        }

        for (int i = 0; i < numins; ++i) {
            std::copy_n(ins[i], sampleframes, inputBuffer[i].begin());
        }

        outputBuffer = combinator->perform(inputBuffer);

        for (int i = 0; i < numouts; ++i) {
            std::copy_n(outputBuffer[i].begin(), sampleframes, outs[i]);
        }
    }

    /**
     * @brief Gets the number of output channels
     * @return Number of output channels based on enabled feature extractors
     */
    [[nodiscard]] int getOutputCount() const noexcept { return outputCount; }

    /**
     * @brief Gets the number of input channels
     * @return Number of input channels (currently fixed at 1)
     */
    [[nodiscard]] constexpr int getInputCount() const noexcept { return inputCount; }

    /**
     * @brief Gets the total number of ports (inlets + outlets)
     * @return Total count of all audio ports
     */
    [[nodiscard]] int getPortCount() const noexcept { return inputCount + outputCount; }

    ~ZerrCombinator() = default;

 private:
    static constexpr int inputCount = 3; /**< Number of signal inlets for receiving audio input */
    int outputCount = 0; /**< Number of signal outlets based on enabled feature extractors */

    zerr::SystemConfigs systemConfigs; /**< System configuration settings */

    zerr::Blocks inputBuffer; /**< Buffer for storing incoming audio samples */
    zerr::Blocks outputBuffer; /**< Multi-channel buffer for storing processed audio samples */

    std::unique_ptr<zerr::EnvelopeCombinator> combinator; /**< Core component that implements the feature extraction algorithms */

    std::string zerr_cfg; /**< Path to configuration file */
};