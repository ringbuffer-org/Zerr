#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "envelopegenerator.h"
#include "logger.h"
#include "ringbuffer.h"
#include "types.h"

// #include "yaml-cpp/yaml.h"

/**
 * @class ZerrFeatures
 * @brief Main wrapper class that interfaces between Max/MSP and the core audio feature extraction functionality
 *
 * This class handles the initialization, audio processing, and cleanup of audio feature extraction operations.
 * It manages the data flow between Max/MSP's audio system and the internal feature processing chain.
 */
class ZerrEnvelopes {
 public:
    /**
     * @brief Creates a new ZerrEnvelopes instance
     * @param sys_config System configuration containing sample rate and block size settings
     * @param ft_names List of audio features to extract from the input signal
     */
    ZerrEnvelopes(const zerr::SystemConfigs& sys_config, std::string selectionMode, std::string spkrCfgFile)
        : systemConfigs { sys_config }
        , selectionMode { selectionMode }
        , spkrCfgFile { spkrCfgFile }
        , inputBuffer(inputCount, std::vector<double>(sys_config.block_size, 0.0))
        , generator { std::make_unique<zerr::EnvelopeGenerator>(sys_config, spkrCfgFile, selectionMode) }
    {
        // post("ZerrEnvelopes::ZerrEnvelopes");
    }

    // Disable copying but allow moving
    ZerrEnvelopes(const ZerrEnvelopes&) = delete;
    ZerrEnvelopes& operator=(const ZerrEnvelopes&) = delete;
    ZerrEnvelopes(ZerrEnvelopes&&) = default;
    ZerrEnvelopes& operator=(ZerrEnvelopes&&) = default;

    /**
     * @brief Initializes all internal components and prepares the object for processing
     * @return true if initialization was successful, false otherwise
     * @throws std::runtime_error if memory allocation fails
     */
    bool initialize()
    {
        // Initialize the Envelope Generator module
        if (!generator->initialize()) {
            post("ZerrEnvelopes::initialize failed to initialize generator");
            return false;
        };

        // assgin Max/MSP print method the the logger function
        // Because we want to see logs in the PD log window
        auto printFunc = [](const std::string& msg) {
            post(msg.c_str());
        };
        generator->setPrinter(printFunc);

        outputCount = generator->getNumSpeakers();
        post("ZerrEnvelopes::initialize outputCount is %d", outputCount);

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

        outputBuffer = generator->perform(inputBuffer);

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

    ~ZerrEnvelopes() = default;

    void setActiveSpeakerIndexs(char* action, int* idxs, size_t size)
    {
        zerr::Indexes indexVec(idxs, idxs + size);
        generator->setActiveSpeakerIndexs(action, indexVec);
    }

    void setCurrentSpeaker(int idx)
    {
        zerr::Index newIdx = idx;
        generator->setCurrentSpeaker(newIdx);
    }

    void setTopoMatrix(char* action, int* idxs, size_t size)
    {
        zerr::Indexes indexVec(idxs, idxs + size);
        generator->setTopoMatrix(action, indexVec);
    }

    void setTrajectoryVector(int* idxs, size_t size)
    {
        zerr::Indexes indexVec(idxs, idxs + size);
        generator->setTrajectoryVector(indexVec);
    }

    void setTriggerInterval(float interval)
    {
        zerr::Param newInterval = interval;
        generator->setTriggerInterval(newInterval);
    }

    void printParameters()
    {
        generator->printParameters();
    }

 private:
    static constexpr int inputCount = 3; /**< Number of signal inlets for receiving audio input */
    int outputCount = 0; /**< Number of signal outlets based on enabled feature extractors */

    zerr::SystemConfigs systemConfigs; /**< System configuration settings */

    zerr::Blocks inputBuffer; /**< Buffer for storing incoming audio samples */
    zerr::Blocks outputBuffer; /**< Multi-channel buffer for storing processed audio samples */

    std::unique_ptr<zerr::EnvelopeGenerator> generator; /**< Core component that implements the feature extraction algorithms */

    std::string spkrCfgFile; /**< Path to speaker configuration file */
    std::string selectionMode; /**< Mode for selecting output speaker routing */

    std::string zerr_cfg; /**< Path to configuration file */
};