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
    for (size_t i = 0; i < activated_features.size(); ++i) {
        std::cout << "  -Name: " << activated_features[i]->get_name() << std::endl;
        std::cout << "  -Category: " << activated_features[i]->get_category() << std::endl;
        std::cout << "  -Description: " << activated_features[i]->get_description() << std::endl;
        std::cout << std::endl;
    }
}

void FeatureBank::initialize(FeatureNames feature_names, SystemConfigs system_configs)
{
    for (auto name : feature_names) {
        activated_features.push_back(_create(name));
    }

    n_features = activated_features.size();
    for (int i = 0; i < n_features; ++i) {
        activated_features[i]->initialize(system_configs);
    }
    y.resize(activated_features.size());

    x.wave.resize(AUDIO_BUFFER_SIZE);
}

FeaturesVals FeatureBank::perform(Block in)
{
    // fetch
    Sample* buf_ptr = nullptr;
    size_t buf_len;
    ring_buffer.enqueue(in);

    x.block.clear();
    x.block = in;

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
    // TODO: use multi-thread
    for (size_t i = 0; i < activated_features.size(); ++i) {
        activated_features[i]->fetch(x);
        activated_features[i]->extract();
        y[i] = activated_features[i]->send();
    }

    // send
    return y;
}

// TODO(Zeyu yang): make this an external function
void FeatureBank::_regist_all()
{
    _regist("rms", []() { return fe_ptr(new RootMeanSquare()); });   // Root Mean Square
    _regist("zcr", []() { return fe_ptr(new ZeroCrossingRate()); }); // Zero Crossing Rate
    _regist("flx", []() { return fe_ptr(new Flux()); });             // Spectral Flux
    _regist("ctd", []() { return fe_ptr(new Centroid()); });         // Spectral Centroid
    _regist("rlf", []() { return fe_ptr(new Rolloff()); });          // Spectral Rolloff
    _regist("cf", []() { return fe_ptr(new CrestFactor()); });       // Crest Factor
    _regist("flt", []() { return fe_ptr(new Flatness()); });         // Spectral Flatness
    _regist("zc", []() { return fe_ptr(new ZeroCrossings()); });     // Zero Crossings
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
