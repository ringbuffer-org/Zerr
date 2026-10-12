#include "featurebank.h"
using namespace zerr;
using namespace feature;

FeatureBank::FeatureBank() : ring_buffer(AUDIO_BUFFER_SIZE), freq_transformer(AUDIO_BUFFER_SIZE)
{
    _regist_all();
}

void FeatureBank::print_info(std::string name)
{
    std::cout << "print_info:\n" << name << std::endl;
}

void FeatureBank::print_all_features()
{
    std::cout << "All registed features: " << std::endl;
    for (const auto& pair : registed_features) {
        std::cout << "  -Name: " << pair.first << std::endl;
    }
}

void FeatureBank::print_active_features()
{
    std::cout << "All activated features: " << std::endl;
    for (const auto& name : active_feature_names) {
        std::cout << "  -Name: " << name << std::endl;
    }
}

void FeatureBank::initialize(FeatureNames feature_names, SystemConfigs system_configs)
{
    active_feature_names = feature_names;
    for (const auto& name : feature_names) {
        activated_features.push_back(_create(name));
    }

    n_features = activated_features.size();
    for (auto& feature : activated_features) {
        feature->initialize(system_configs);
    }
    y.resize(activated_features.size());

    x.wave.resize(AUDIO_BUFFER_SIZE);
}

FeaturesVals FeatureBank::perform(Samples in)
{
    // fetch
    Sample* buf_ptr = nullptr;
    size_t buf_len;
    ring_buffer.enqueue(in);

    x.block = std::move(in);

    buf_ptr = x.wave.data();
    buf_len = x.wave.size();
    ring_buffer.get_samples(buf_ptr, buf_len);

    buf_ptr = freq_transformer.fft_input();
    buf_len = freq_transformer.get_frame_size();
    ring_buffer.get_samples(buf_ptr, buf_len);
    freq_transformer.windowing();
    freq_transformer.fft();
    freq_transformer.power_spectrum();
    x.spec = freq_transformer.get_power_spectrum();

    // process:
    for (size_t i = 0; i < activated_features.size(); ++i) {
        activated_features[i]->fetch(x);
        activated_features[i]->extract();
        y[i] = activated_features[i]->send();
    }

    // send
    return y;
}

namespace {
struct Registration {
    const char* name;
    FeatureBank::CreateFunc create;
};

// The one table of feature extractors. Adding a feature is one line here.
const Registration kRegistry[] = {
    {"rms", []() { return FeatureBank::fe_ptr(new RootMeanSquare()); }},   // Root Mean Square
    {"zcr", []() { return FeatureBank::fe_ptr(new ZeroCrossingRate()); }}, // Zero Crossing Rate
    {"flx", []() { return FeatureBank::fe_ptr(new Flux()); }},             // Spectral Flux
    {"ctd", []() { return FeatureBank::fe_ptr(new Centroid()); }},         // Spectral Centroid
    {"rlf", []() { return FeatureBank::fe_ptr(new Rolloff()); }},          // Spectral Rolloff
    {"cf", []() { return FeatureBank::fe_ptr(new CrestFactor()); }},       // Crest Factor
    {"flt", []() { return FeatureBank::fe_ptr(new Flatness()); }},         // Spectral Flatness
    {"zc", []() { return FeatureBank::fe_ptr(new ZeroCrossings()); }},     // Zero Crossings
};
} // namespace

void FeatureBank::_regist_all()
{
    for (const auto& entry : kRegistry) {
        _regist(entry.name, entry.create);
    }
}

FeatureNames FeatureBank::availableFeatures()
{
    FeatureNames names;
    for (const auto& entry : kRegistry) {
        names.emplace_back(entry.name);
    }
    return names;
}

void FeatureBank::_regist(const std::string& className, CreateFunc createFunc)
{
    registed_features[className] = createFunc;
}

std::unique_ptr<FeatureExtractor> FeatureBank::_create(const std::string& className)
{
    auto it = registed_features.find(className);
    if (it != registed_features.end()) {
        return it->second();
    }

    throw std::runtime_error("Feature |" + className + "| not found, please check your spelling");
}
