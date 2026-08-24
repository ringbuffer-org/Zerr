/**
 * @file zerr_jack.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Zerr* standalone JACK client header
 *
 *        Wires the full core pipeline into a single JACK client:
 *        FeatureBank -> EnvelopeGenerator(s) -> EnvelopeCombinator -> AudioDisperser.
 *        One audio input port, one output port per speaker in the speaker config.
 * @date 2026-08-19
 *
 * @copyright Copyright (c) 2023-2026
 */
#ifndef ZERR_JACK_H
#define ZERR_JACK_H

#include <atomic>
#include <memory>
#include <string>
#include <vector>

#include <jack/jack.h>

#include "audiodisperser.h"
#include "envelopecombinator.h"
#include "envelopegenerator.h"
#include "featurebank.h"
#include "types.h"

namespace zerr {

/**
 * @brief One control input of an EnvelopeGenerator.
 *
 * Each of the generator's three inlets (main, spread, volume) is driven either by
 * an extracted feature or by a fixed value held for the whole run.
 */
struct ControlSource {
    bool isConstant     = true; /**< true: use `constant`, false: use feature `featureIndex` */
    Param constant      = 0.0f; /**< Fixed value used when isConstant */
    size_t featureIndex = 0;    /**< Index into the activated feature list otherwise */
};

/**
 * @brief Configuration of a single envelope source (one EnvelopeGenerator).
 */
struct EnvelopeSourceConfig {
    GenMode mode = GenMode::Trajectory; /**< trigger | trajectory */
    ControlSource main;                 /**< Inlet 1: trigger signal or trajectory position */
    ControlSource spread;               /**< Inlet 2: spread amount, 0.0-1.0 */
    ControlSource volume;               /**< Inlet 3: output volume, 0.0-1.0 */
};

/**
 * @brief Everything the client needs, as parsed from the client YAML config.
 */
struct ClientConfig {
    std::string clientName = "zerr";           /**< JACK client name */
    ConfigPath speakerCfgFile;                 /**< Path to the speaker array YAML */
    FeatureNames featureNames;                 /**< Features to activate, in order */
    std::vector<EnvelopeSourceConfig> sources; /**< One or more envelope generators */
    std::string combinationMode = "max"; /**< add | root | max, used when sources.size() > 1 */
    bool autoConnect            = false; /**< Connect to the physical ports on activation */

    /**
     * @brief Parse a client config from a YAML file.
     *
     * Relative paths inside the file (the speaker config) resolve against the
     * directory holding the config file, not the working directory.
     *
     * @param path Path to the client YAML config
     * @return ClientConfig The parsed configuration
     * @throws std::runtime_error on a missing file, a malformed entry, or a
     *         control input naming a feature that is not activated
     */
    static ClientConfig fromFile(const std::string& path);
};

/**
 * @class ZerrJack
 * @brief Standalone JACK client running the Zerr* spatialization pipeline.
 *
 * Lifetime is open() -> initialize() -> run() -> destructor. The JACK process
 * callback owns the whole chain; no parameter can be changed while it runs.
 */
class ZerrJack {
  public:
    /**
     * @brief Construct the client. Does not touch JACK yet.
     * @param config The parsed client configuration
     */
    explicit ZerrJack(ClientConfig config);

    /**
     * @brief Open the JACK client, build the pipeline and register the ports.
     *
     * The block size and sample rate the core modules are sized against are taken
     * from the JACK server here, which is why this must run before any processing.
     *
     * @return bool whether the client is ready to be activated
     */
    bool initialize();

    /**
     * @brief Activate the client and block until stopped.
     *
     * Returns when requestStop() is called, when the JACK server shuts down, or
     * when the server changes a parameter the pipeline was sized against.
     *
     * @return bool false if the run ended because of an error
     */
    bool run();

    /**
     * @brief Ask run() to return. Safe to call from a signal handler.
     */
    void requestStop() noexcept { stopRequested.store(true, std::memory_order_relaxed); }

    ~ZerrJack();

    ZerrJack(const ZerrJack&)            = delete;
    ZerrJack& operator=(const ZerrJack&) = delete;

  private:
    ClientConfig config;      /**< Parsed configuration, fixed for the client's lifetime */
    SystemConfigs systemCfgs; /**< Sample rate and block size, as reported by JACK */

    jack_client_t* client  = nullptr;      /**< The JACK client handle */
    jack_port_t* inputPort = nullptr;      /**< Single audio input port */
    std::vector<jack_port_t*> outputPorts; /**< One output port per speaker */

    int numSpeakers = 0; /**< Number of speakers, from the speaker config */

    // pipeline modules
    std::unique_ptr<FeatureBank> bank;
    std::vector<std::unique_ptr<EnvelopeGenerator>> generators;
    std::unique_ptr<EnvelopeCombinator> combinator; /**< nullptr when there is one source */
    std::unique_ptr<AudioDisperser> disperser;

    // preallocated buffers: the process callback must not size any of these
    Samples audioBlock;       /**< Copy of the input block, kept for the disperser */
    Samples featureInput;     /**< Input block handed to the FeatureBank (it moves from it) */
    FeaturesVals featureVals; /**< Feature values for the current block */
    Blocks envelopeInput;     /**< 3 x blockSize control signals for one generator */
    Blocks combinatorInput;   /**< numSource * numSpeakers envelope blocks */
    Blocks envelopes;         /**< numSpeakers combined envelopes */
    Blocks disperserInput;    /**< 1 + numSpeakers blocks: source audio then envelopes */
    Blocks disperserOutput;   /**< numSpeakers output blocks */

    std::atomic<bool> stopRequested{false};  /**< Set by requestStop() */
    std::atomic<bool> serverGone{false};     /**< Set by the JACK shutdown callback */
    std::atomic<bool> configInvalid{false};  /**< Set when JACK changes block size/sample rate */
    std::atomic<unsigned long> xrunCount{0}; /**< Number of xruns reported by JACK */

    /**
     * @brief Build and initialize the core modules against systemCfgs.
     * @return bool whether every module initialized
     */
    bool _buildPipeline();
    /**
     * @brief Register the input port and one output port per speaker.
     * @return bool whether every port was registered
     */
    bool _registerPorts();
    /**
     * @brief Size every processing buffer. Called once, before activation.
     */
    bool _allocateBuffers();
    /**
     * @brief Connect the ports to the first physical capture/playback ports.
     *
     * Best effort: a failure to connect is reported but does not stop the client.
     */
    void _autoConnect();
    /**
     * @brief Fill one control block from a ControlSource.
     * @param source The control input description
     * @param dst The block to fill, already sized to the block size
     */
    void _fillControlBlock(const ControlSource& source, Samples& dst);

    /**
     * @brief The real process callback. Runs in the JACK realtime thread.
     * @param nframes Number of frames in this block
     * @return int 0 on success, as JACK expects
     */
    int _process(jack_nframes_t nframes);
    /**
     * @brief Write silence to every output port. Used while the client is not usable.
     */
    void _silence(jack_nframes_t nframes);

    // JACK C callbacks, each forwarding to the instance in `arg`
    static int _processCallback(jack_nframes_t nframes, void* arg);
    static int _bufferSizeCallback(jack_nframes_t nframes, void* arg);
    static int _sampleRateCallback(jack_nframes_t nframes, void* arg);
    static int _xrunCallback(void* arg);
    static void _shutdownCallback(void* arg);
};

} // namespace zerr
#endif // ZERR_JACK_H
