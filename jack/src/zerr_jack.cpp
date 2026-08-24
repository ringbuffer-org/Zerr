/**
 * @file zerr_jack.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Zerr* standalone JACK client implementation
 * @date 2026-08-19
 *
 * @copyright Copyright (c) 2023-2026
 */
#include "zerr_jack.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <thread>

#include <yaml-cpp/yaml.h>

using namespace zerr;

namespace {

/**
 * @brief Directory part of a path, or "." when there is none.
 *
 * Only ever applied to the config path the user typed, so a POSIX-style split is
 * enough -- JACK does not run on Windows, and neither does this target.
 */
std::string dirnameOf(const std::string& path)
{
    const size_t slash = path.find_last_of('/');
    return slash == std::string::npos ? std::string(".") : path.substr(0, slash);
}

/**
 * @brief Resolve `path` against `base` unless it is already absolute.
 */
std::string resolvePath(const std::string& base, const std::string& path)
{
    if (path.empty() || path[0] == '/') {
        return path;
    }
    return base + "/" + path;
}

/**
 * @brief Parse one control input entry: either a number or an activated feature name.
 *
 * The number/name split is done with strtod rather than by catching a yaml-cpp
 * conversion failure, so a feature name that happens to start with a digit is
 * rejected outright instead of silently becoming a constant.
 */
ControlSource parseControl(const YAML::Node& node, const std::string& field,
                           const FeatureNames& featureNames)
{
    if (!node) {
        throw std::runtime_error("envelope source is missing the '" + field + "' entry");
    }

    const std::string raw = node.as<std::string>();

    char* end          = nullptr;
    const double value = std::strtod(raw.c_str(), &end);
    if (end != raw.c_str() && *end == '\0') {
        ControlSource control;
        control.isConstant = true;
        control.constant   = static_cast<Param>(value);
        return control;
    }

    const auto it = std::find(featureNames.begin(), featureNames.end(), raw);
    if (it == featureNames.end()) {
        throw std::runtime_error("'" + field + ": " + raw +
                                 "' names a feature that is not in the 'features' list");
    }

    ControlSource control;
    control.isConstant   = false;
    control.featureIndex = static_cast<size_t>(std::distance(featureNames.begin(), it));
    return control;
}

} // namespace

ClientConfig ClientConfig::fromFile(const std::string& path)
{
    YAML::Node root;
    try {
        root = YAML::LoadFile(path);
    }
    catch (const YAML::Exception& e) {
        throw std::runtime_error("cannot read client config " + path + ": " + e.what());
    }

    ClientConfig config;
    const std::string base = dirnameOf(path);

    if (const YAML::Node client = root["client"]) {
        if (client["name"]) {
            config.clientName = client["name"].as<std::string>();
        }
        if (client["auto_connect"]) {
            config.autoConnect = client["auto_connect"].as<bool>();
        }
    }

    if (!root["speaker_config"]) {
        throw std::runtime_error(path + ": missing 'speaker_config'");
    }
    config.speakerCfgFile = resolvePath(base, root["speaker_config"].as<std::string>());

    if (!root["features"] || !root["features"].IsSequence() || root["features"].size() == 0) {
        throw std::runtime_error(path + ": 'features' must be a non-empty list");
    }
    for (const auto& feature : root["features"]) {
        config.featureNames.push_back(feature.as<std::string>());
    }

    const YAML::Node envelope = root["envelope"];
    if (!envelope) {
        throw std::runtime_error(path + ": missing 'envelope'");
    }
    if (envelope["combination_mode"]) {
        config.combinationMode = envelope["combination_mode"].as<std::string>();
    }

    const YAML::Node sources = envelope["sources"];
    if (!sources || !sources.IsSequence() || sources.size() == 0) {
        throw std::runtime_error(path + ": 'envelope.sources' must be a non-empty list");
    }
    for (const auto& source : sources) {
        EnvelopeSourceConfig sourceCfg;
        sourceCfg.mode   = parseGenMode(source["mode"] ? source["mode"].as<std::string>()
                                                       : std::string("trajectory"));
        sourceCfg.main   = parseControl(source["main"], "main", config.featureNames);
        sourceCfg.spread = parseControl(source["spread"], "spread", config.featureNames);
        sourceCfg.volume = parseControl(source["volume"], "volume", config.featureNames);
        config.sources.push_back(sourceCfg);
    }

    return config;
}

ZerrJack::ZerrJack(ClientConfig config) : config(std::move(config)) {}

bool ZerrJack::initialize()
{
    jack_status_t status;
    client = jack_client_open(config.clientName.c_str(), JackNoStartServer, &status);
    if (!client) {
        std::cerr << "zerr_jack: cannot connect to the JACK server (status 0x" << std::hex
                  << static_cast<int>(status) << std::dec << ")." << std::endl;
        std::cerr << "  Is jackd running? Start it before launching this client." << std::endl;
        return false;
    }
    if (status & JackNameNotUnique) {
        std::cout << "zerr_jack: client name taken, registered as '" << jack_get_client_name(client)
                  << "' instead." << std::endl;
    }

    // Every core module sizes its buffers once, at initialize(), against these two
    // numbers -- so they have to come from the server before the pipeline is built.
    systemCfgs.sample_rate = jack_get_sample_rate(client);
    systemCfgs.block_size  = jack_get_buffer_size(client);

    if (!_buildPipeline()) {
        return false;
    }
    if (!_allocateBuffers()) {
        return false;
    }
    if (!_registerPorts()) {
        return false;
    }

    jack_set_process_callback(client, &ZerrJack::_processCallback, this);
    jack_set_buffer_size_callback(client, &ZerrJack::_bufferSizeCallback, this);
    jack_set_sample_rate_callback(client, &ZerrJack::_sampleRateCallback, this);
    jack_set_xrun_callback(client, &ZerrJack::_xrunCallback, this);
    jack_on_shutdown(client, &ZerrJack::_shutdownCallback, this);

    std::cout << "zerr_jack: " << systemCfgs.sample_rate << " Hz, " << systemCfgs.block_size
              << " frames/block, " << config.sources.size() << " envelope source(s), "
              << numSpeakers << " speakers." << std::endl;

    return true;
}

bool ZerrJack::_buildPipeline()
{
    bank = std::make_unique<FeatureBank>();
    try {
        bank->initialize(config.featureNames, systemCfgs);
    }
    catch (const std::exception& e) {
        std::cerr << "zerr_jack: cannot activate the requested features: " << e.what() << std::endl;
        std::cerr << "  Run with --list-features to see the supported names." << std::endl;
        return false;
    }

    for (const auto& sourceCfg : config.sources) {
        auto generator =
            std::make_unique<EnvelopeGenerator>(systemCfgs, config.speakerCfgFile, sourceCfg.mode);
        if (!generator->initialize()) {
            std::cerr << "zerr_jack: EnvelopeGenerator failed to initialize from "
                      << config.speakerCfgFile << std::endl;
            return false;
        }

        // Read the count off the generator rather than its numOutlet member: that
        // member is shadowed by a local in EnvelopeGenerator::initialize() and so
        // never assigned. See docs/design/repo-audit-2026-07-30.md.
        const int speakers = generator->getNumSpeakers();
        if (numSpeakers == 0) {
            numSpeakers = speakers;
        }
        else if (speakers != numSpeakers) {
            std::cerr << "zerr_jack: envelope sources disagree on the speaker count ("
                      << numSpeakers << " vs " << speakers << ")." << std::endl;
            return false;
        }

        generators.push_back(std::move(generator));
    }

    if (numSpeakers <= 0) {
        std::cerr << "zerr_jack: the speaker config declares no speakers." << std::endl;
        return false;
    }

    // A single source has nothing to combine with, so the combinator is left out
    // of the chain entirely rather than run as an identity pass over every block.
    if (generators.size() > 1) {
        combinator = std::make_unique<EnvelopeCombinator>(
            static_cast<int>(generators.size()), numSpeakers, systemCfgs, config.combinationMode);
        if (!combinator->initialize()) {
            std::cerr << "zerr_jack: unknown envelope combination mode '" << config.combinationMode
                      << "' (expected add, root or max)." << std::endl;
            return false;
        }
    }

    disperser = std::make_unique<AudioDisperser>(numSpeakers, systemCfgs);
    if (!disperser->initialize()) {
        std::cerr << "zerr_jack: AudioDisperser failed to initialize." << std::endl;
        return false;
    }

    return true;
}

bool ZerrJack::_allocateBuffers()
{
    const size_t blockSize = systemCfgs.block_size;

    audioBlock.assign(blockSize, 0.0);
    featureInput.assign(blockSize, 0.0);
    featureVals.assign(config.featureNames.size(), FeatureVals(blockSize, 0.0f));

    envelopeInput.assign(3, Samples(blockSize, 0.0));
    envelopes.assign(numSpeakers, Samples(blockSize, 0.0));
    if (combinator) {
        combinatorInput.assign(generators.size() * numSpeakers, Samples(blockSize, 0.0));
    }

    disperserInput.assign(numSpeakers + 1, Samples(blockSize, 0.0));
    disperserOutput.assign(numSpeakers, Samples(blockSize, 0.0));

    return true;
}

bool ZerrJack::_registerPorts()
{
    inputPort = jack_port_register(client, "in", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    if (!inputPort) {
        std::cerr << "zerr_jack: cannot register the input port." << std::endl;
        return false;
    }

    outputPorts.resize(numSpeakers, nullptr);
    for (int i = 0; i < numSpeakers; ++i) {
        const std::string name = "out_" + std::to_string(i + 1);
        outputPorts[i] =
            jack_port_register(client, name.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
        if (!outputPorts[i]) {
            std::cerr << "zerr_jack: cannot register output port " << name
                      << " (JACK allows a limited number of ports per client)." << std::endl;
            return false;
        }
    }

    return true;
}

void ZerrJack::_autoConnect()
{
    const char** capture = jack_get_ports(client, nullptr, JACK_DEFAULT_AUDIO_TYPE,
                                          JackPortIsPhysical | JackPortIsOutput);
    if (capture && capture[0]) {
        if (jack_connect(client, capture[0], jack_port_name(inputPort)) != 0) {
            std::cerr << "zerr_jack: could not connect " << capture[0] << " to the input port."
                      << std::endl;
        }
    }
    else {
        std::cerr << "zerr_jack: no physical capture port to connect the input to." << std::endl;
    }
    if (capture) {
        jack_free(capture);
    }

    const char** playback = jack_get_ports(client, nullptr, JACK_DEFAULT_AUDIO_TYPE,
                                           JackPortIsPhysical | JackPortIsInput);
    if (!playback) {
        std::cerr << "zerr_jack: no physical playback ports to connect the outputs to."
                  << std::endl;
        return;
    }

    int connected = 0;
    for (int i = 0; i < numSpeakers && playback[i]; ++i) {
        if (jack_connect(client, jack_port_name(outputPorts[i]), playback[i]) == 0) {
            ++connected;
        }
    }
    if (connected < numSpeakers) {
        std::cout << "zerr_jack: connected " << connected << " of " << numSpeakers
                  << " outputs; the rest have no physical port to land on." << std::endl;
    }
    jack_free(playback);
}

bool ZerrJack::run()
{
    if (jack_activate(client) != 0) {
        std::cerr << "zerr_jack: cannot activate the client." << std::endl;
        return false;
    }

    if (config.autoConnect) {
        _autoConnect();
    }

    std::cout << "zerr_jack: running. Press Ctrl+C to stop." << std::endl;

    bool clean = true;
    while (!stopRequested.load(std::memory_order_relaxed)) {
        if (serverGone.load(std::memory_order_relaxed)) {
            std::cerr << "zerr_jack: the JACK server shut down." << std::endl;
            clean = false;
            break;
        }
        if (configInvalid.load(std::memory_order_relaxed)) {
            std::cerr << "zerr_jack: the server changed the block size or sample rate the "
                         "pipeline was built for; restart the client to pick it up."
                      << std::endl;
            clean = false;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    const unsigned long xruns = xrunCount.load(std::memory_order_relaxed);
    if (xruns > 0) {
        std::cout << "zerr_jack: " << xruns << " xrun(s) during this run." << std::endl;
    }

    return clean;
}

void ZerrJack::_fillControlBlock(const ControlSource& source, Samples& dst)
{
    if (source.isConstant) {
        std::fill(dst.begin(), dst.end(), static_cast<Sample>(source.constant));
        return;
    }

    const FeatureVals& values = featureVals[source.featureIndex];
    const size_t shared       = std::min(dst.size(), values.size());
    for (size_t i = 0; i < shared; ++i) {
        dst[i] = static_cast<Sample>(values[i]);
    }
    for (size_t i = shared; i < dst.size(); ++i) {
        dst[i] = 0.0;
    }
}

int ZerrJack::_process(jack_nframes_t nframes)
{
    // The modules were sized for one specific block size; anything else, and the
    // block is dropped rather than written past the end of a buffer.
    if (nframes != systemCfgs.block_size || configInvalid.load(std::memory_order_relaxed)) {
        _silence(nframes);
        return 0;
    }

    const auto* in =
        static_cast<const jack_default_audio_sample_t*>(jack_port_get_buffer(inputPort, nframes));
    for (jack_nframes_t i = 0; i < nframes; ++i) {
        audioBlock[i] = static_cast<Sample>(in[i]);
    }

    // NOTE: the core's perform() methods return their buffers by value, so this
    // path allocates on the realtime thread -- exactly as the PureData and Max/MSP
    // wrappers do. Raise the JACK buffer size if it shows up as xruns.
    featureInput = audioBlock; // FeatureBank::perform moves from its argument
    featureVals  = bank->perform(featureInput);

    if (combinator) {
        for (size_t s = 0; s < generators.size(); ++s) {
            _fillControlBlock(config.sources[s].main, envelopeInput[0]);
            _fillControlBlock(config.sources[s].spread, envelopeInput[1]);
            _fillControlBlock(config.sources[s].volume, envelopeInput[2]);

            const Blocks sourceEnvelopes = generators[s]->perform(envelopeInput);
            if (sourceEnvelopes.size() < static_cast<size_t>(numSpeakers)) {
                _silence(nframes);
                return 0;
            }
            for (int c = 0; c < numSpeakers; ++c) {
                combinatorInput[c + s * numSpeakers] = sourceEnvelopes[c];
            }
        }
        envelopes = combinator->perform(combinatorInput);
    }
    else {
        _fillControlBlock(config.sources[0].main, envelopeInput[0]);
        _fillControlBlock(config.sources[0].spread, envelopeInput[1]);
        _fillControlBlock(config.sources[0].volume, envelopeInput[2]);

        envelopes = generators[0]->perform(envelopeInput);
    }

    if (envelopes.size() < static_cast<size_t>(numSpeakers)) {
        _silence(nframes);
        return 0;
    }

    disperserInput[0] = audioBlock;
    for (int c = 0; c < numSpeakers; ++c) {
        disperserInput[c + 1] = envelopes[c];
    }
    disperserOutput = disperser->perfrom(disperserInput);

    for (int c = 0; c < numSpeakers; ++c) {
        auto* out = static_cast<jack_default_audio_sample_t*>(
            jack_port_get_buffer(outputPorts[c], nframes));
        const Samples& block = disperserOutput[c];
        for (jack_nframes_t i = 0; i < nframes; ++i) {
            out[i] = static_cast<jack_default_audio_sample_t>(block[i]);
        }
    }

    return 0;
}

void ZerrJack::_silence(jack_nframes_t nframes)
{
    for (auto* port : outputPorts) {
        auto* out = static_cast<jack_default_audio_sample_t*>(jack_port_get_buffer(port, nframes));
        std::memset(out, 0, sizeof(jack_default_audio_sample_t) * nframes);
    }
}

int ZerrJack::_processCallback(jack_nframes_t nframes, void* arg)
{
    return static_cast<ZerrJack*>(arg)->_process(nframes);
}

int ZerrJack::_bufferSizeCallback(jack_nframes_t nframes, void* arg)
{
    auto* self = static_cast<ZerrJack*>(arg);
    if (nframes != self->systemCfgs.block_size) {
        self->configInvalid.store(true, std::memory_order_relaxed);
    }
    return 0;
}

int ZerrJack::_sampleRateCallback(jack_nframes_t nframes, void* arg)
{
    auto* self = static_cast<ZerrJack*>(arg);
    if (nframes != self->systemCfgs.sample_rate) {
        self->configInvalid.store(true, std::memory_order_relaxed);
    }
    return 0;
}

int ZerrJack::_xrunCallback(void* arg)
{
    static_cast<ZerrJack*>(arg)->xrunCount.fetch_add(1, std::memory_order_relaxed);
    return 0;
}

void ZerrJack::_shutdownCallback(void* arg)
{
    static_cast<ZerrJack*>(arg)->serverGone.store(true, std::memory_order_relaxed);
}

ZerrJack::~ZerrJack()
{
    if (client) {
        // Deactivate before the modules go away: the process callback dereferences
        // them, and jack_client_close alone does not guarantee it has stopped.
        if (!serverGone.load(std::memory_order_relaxed)) {
            jack_deactivate(client);
        }
        jack_client_close(client);
        client = nullptr;
    }
}
