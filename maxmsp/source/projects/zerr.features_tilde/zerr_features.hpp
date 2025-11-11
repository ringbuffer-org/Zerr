/**
 * @file    zerr_features.hpp
 * @author  Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief   mc.zerr.features~ Max/MSP External using Max API for better multi-channel support
 * @date    2025-05-01
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

#include "featurebank.h"


/**
 * @class ZerrFeatures
 * @brief Main wrapper class that interfaces between Max/MSP and the core audio feature extraction functionality
 *
 * This class handles the initialization, audio processing, and cleanup of audio feature extraction operations.
 * It manages the data flow between Max/MSP's audio system and the internal feature processing chain.
 */
class ZerrFeatures {
 public:
    /**
     * @brief Creates a new ZerrFeatures instance
     * @param sys_config System configuration containing sample rate and block size settings
     * @param ft_names List of audio features to extract from the input signal
     */
    explicit ZerrFeatures(float sampleRate, int blockSize, const zerr::FeatureNames& names)
        : systemConfigs {
            .sample_rate = (size_t)sampleRate,
            .block_size = (size_t)blockSize,
        }
        , inputBuffer(inputCount, std::vector<double>(blockSize, 0.0))
        , bank { std::make_unique<zerr::FeatureBank>() }
        , featureNames { std::move(names) } // Move instead of copy
    {
    }

    // Disable copying but allow moving
    ZerrFeatures(const ZerrFeatures&) = delete;
    ZerrFeatures& operator=(const ZerrFeatures&) = delete;
    ZerrFeatures(ZerrFeatures&&) = delete;
    ZerrFeatures& operator=(ZerrFeatures&&) = delete;

    /**
     * @brief Initializes all internal components and prepares the object for processing
     * @return true if initialization was successful, false otherwise
     * @throws std::runtime_error if memory allocation fails
     */
    bool initialize()
    {
        try {
            bank->initialize(featureNames, systemConfigs);
        } catch (const std::exception& e) {
            return false;
        }

        outputCount = featureNames.size();

        inputBuffer.resize(inputCount, std::vector<double>(systemConfigs.block_size, 0.0));
        outputBuffer.resize(outputCount);

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
        // Validate input parameters
        if (!ins || !outs || numins < inputCount || numouts < outputCount) {
            throw std::invalid_argument("Invalid buffer pointers or sizes in perform()");
        }

        // Use std::copy for better optimization possibilities
        for (int i = 0; i < numins; ++i) {
            std::copy_n(ins[i], sampleframes, inputBuffer[i].begin());
        }

        // Process audio through the feature bank
        outputBuffer = bank->perform(inputBuffer[0]);

        // Copy output to destination buffers
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

    ~ZerrFeatures() = default;

 private:
    static constexpr int inputCount = 1; /**< Number of signal inlets for receiving audio input */
    int outputCount = 0; /**< Number of signal outlets based on enabled feature extractors */

    zerr::SystemConfigs systemConfigs; /**< System configuration settings */
    zerr::FeatureNames featureNames; /**< List of enabled audio feature extractors */

    zerr::Blocks inputBuffer; /**< Buffer for storing incoming audio samples */
    zerr::FeaturesVals outputBuffer; /**< Buffer for storing extracted feature values */

    std::unique_ptr<zerr::FeatureBank> bank; /**< Core component that implements the feature extraction algorithms */
};