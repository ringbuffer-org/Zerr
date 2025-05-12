#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "envelopegenerator.h"
// #include "logger.h"
// #include "ringbuffer.h"
// #include "types.h"

/**
 * @class ZerrEnvelopes
 * @brief Main wrapper class that interfaces between Max/MSP and the core multi-channel envelope generation functionality
 *
 * This class handles the initialization, audio processing, and cleanup of audio feature extraction operations.
 * It manages the data flow between Max/MSP's audio system and the internal feature processing chain.
 */
class ZerrEnvelopes {
 public:
    /**
     * @brief Creates a new ZerrEnvelopes instance
     * @param sampleRate The audio sample rate in Hz
     * @param BlockSize The audio processing block size in samples
     * @param selectionMode The envelope generation mode ("trajectory" or "trigger")
     * @param spkrCfgFile Path to the speaker configuration file
     */
    ZerrEnvelopes(float sampleRate, int BlockSize, std::string selectionMode, std::string spkrCfgFile)
        : systemConfigs {
            .sample_rate = (size_t)sampleRate,
            .block_size = (size_t)BlockSize,
        }
        , selectionMode { selectionMode }
        , spkrCfgFile { spkrCfgFile }
        , generator { std::make_unique<zerr::EnvelopeGenerator>(systemConfigs, spkrCfgFile, selectionMode) }
    {
    }

    // Disable copying but allow moving
    ZerrEnvelopes(const ZerrEnvelopes&) = delete;
    ZerrEnvelopes& operator=(const ZerrEnvelopes&) = delete;
    ZerrEnvelopes(ZerrEnvelopes&&) = default;
    ZerrEnvelopes& operator=(ZerrEnvelopes&&) = default;

    /**
     * @brief Initializes all internal components and prepares the object for processing
     * @return true if initialization was successful, false otherwise
     */
    bool initialize()
    {
        // Initialize the Envelope Generator module
        if (!generator->initialize()) {
            // post("ZerrEnvelopes::initialize failed to initialize generator");
            return false;
        };

        // assgin Max/MSP print method the the logger function
        // Because we want to see logs in the PD log window
        auto printFunc = [](const std::string& msg) {
            post(msg.c_str());
        };
        generator->setPrinter(printFunc);

        outputCount = generator->getNumSpeakers();
        // post("ZerrEnvelopes::initialize outputCount is %d", outputCount);

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
     * @throws std::invalid_argument if buffer pointers or sizes are invalid
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
     * @return Number of output channels corresponding to the speaker setup
     */
    [[nodiscard]] int getOutputCount() const noexcept { return outputCount; }

    /**
     * @brief Gets the number of input channels
     * @return Number of input channels (fixed at 3: main, spread, volume)
     */
    [[nodiscard]] constexpr int getInputCount() const noexcept { return inputCount; }

    /**
     * @brief Gets the total number of ports (inlets + outlets)
     * @return Total count of all audio ports
     */
    [[nodiscard]] int getPortCount() const noexcept { return inputCount + outputCount; }

    ~ZerrEnvelopes() = default;

    /**
     * @brief Sets the active speaker indices with specified action
     * @param action Action to perform: "add", "del", or "set"
     * @param idxs Array of speaker indices
     * @param size Number of indices in the array
     */
    void setActiveSpeakerIndexs(char* action, int* idxs, size_t size)
    {
        zerr::Indexes indexVec(idxs, idxs + size);
        generator->setActiveSpeakerIndexs(action, indexVec);
    }

    /**
     * @brief Sets the current active speaker
     * @param idx Index of the speaker to set as current
     */
    void setCurrentSpeaker(int idx)
    {
        zerr::Index newIdx = idx;
        generator->setCurrentSpeaker(newIdx);
    }

    /**
     * @brief Updates the topology matrix with specified action
     * @param action Action to perform: "add", "del", or "set"
     * @param idxs Array of topology connections
     * @param size Number of indices in the array
     */
    void setTopoMatrix(char* action, int* idxs, size_t size)
    {
        zerr::Indexes indexVec(idxs, idxs + size);
        generator->setTopoMatrix(action, indexVec);
    }

    /**
     * @brief Sets the trajectory vector for envelope movement
     * @param idxs Array of speaker indices defining the trajectory
     * @param size Number of indices in the array
     */
    void setTrajectoryVector(int* idxs, size_t size)
    {
        zerr::Indexes indexVec(idxs, idxs + size);
        generator->setTrajectoryVector(indexVec);
    }

    /**
     * @brief Sets the trigger interval for envelope triggering
     * @param interval Time interval in milliseconds
     */
    void setTriggerInterval(float interval)
    {
        zerr::Param newInterval = interval;
        generator->setTriggerInterval(newInterval);
    }

    /**
     * @brief Prints current parameters to the console
     */
    void printParameters()
    {
        generator->printParameters();
    }

 private:
    static constexpr int inputCount = 3; /**< Number of signal inlets: main(0), spread(1), volume(2) */
    int outputCount = 0; /**< Number of signal outlets based on the loudspeaker setup */

    zerr::SystemConfigs systemConfigs; /**< System configuration: sample rate and block size */

    zerr::Blocks inputBuffer; /**< Multi-channel buffer for storing incoming audio samples */
    zerr::Blocks outputBuffer; /**< Multi-channel buffer for storing envelope outputs */

    std::unique_ptr<zerr::EnvelopeGenerator> generator; /**< Core component that implements envelope generation logic */

    std::string spkrCfgFile; /**< Path to speaker configuration YAML file */
    std::string selectionMode; /**< Envelope generation mode: "trajectory" or "trigger" */
};