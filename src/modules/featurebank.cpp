#include "featurebank.h"
using namespace zerr;
using namespace feature;


FeatureBank::FeatureBank():ring_buffer(AUDIO_BUFFER_SIZE),freq_transformer(AUDIO_BUFFER_SIZE){
    _regist_all();
}


// void FeatureBank::setup(std::string name){
//     activated_features.push_back(_create(name));
// }


// void FeatureBank::shutdown(std::string name){
//     std::cout<<"shutdown:\n"<<name<<std::endl;
// }


void FeatureBank::print_info(std::string name){
    std::cout<<"print_info:\n"<<name<<std::endl;
}


void FeatureBank::print_all_features(){
    std::cout<<"All registed features: "<<std::endl;
    for (const auto& pair : registed_features) {
        std::cout<<"  -Name: "<<pair.first<<std::endl;
    }
}


void FeatureBank::print_active_features(){
    std::cout<<"All activated features: "<<std::endl;
    for (int i = 0; i < activated_features.size(); ++i){
        std::cout<<"  -Name: "<<activated_features[i]->get_name()<<std::endl;
        std::cout<<"  -Category: "<<activated_features[i]->get_category()<<std::endl;
        std::cout<<"  -Description: "<<activated_features[i]->get_description()<<std::endl;
    }
}


void FeatureBank::initialize(t_featureNameList feature_names, t_systemConfigs system_configs){
    for (auto name : feature_names) {
        activated_features.push_back(_create(name));
    }

    n_features = activated_features.size();
    for (int i = 0; i < n_features; ++i){
        activated_features[i]->initialize(system_configs);
    }
    y.resize(activated_features.size());

    x.wave.resize(AUDIO_BUFFER_SIZE);
}


void FeatureBank::fetch(t_blockIn in){
    t_sample* buf_ptr=nullptr;
    size_t buf_len;
    ring_buffer.enqueue(in);

    x.blck.clear();
    x.blck = in;

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
}


void FeatureBank::process(){
    for (int i = 0; i < activated_features.size(); ++i){
        activated_features[i]->fetch(x);
        activated_features[i]->extract();
        y[i] = activated_features[i]->send();
    }
}


t_featureValueList FeatureBank::send(){

    #ifdef TESTMODE
    std::cout<<y[0].normalized<<"  "<<y[1].original<<std::endl;
    #endif // TESTMODE

    return y;
}

// make this an external function
void FeatureBank::_regist_all(){
    _regist("Centroid", []() {
        return fe_ptr(new Centroid());
    });
    _regist("ZeroCrossingRate", []() {
        return fe_ptr(new ZeroCrossingRate());
    }); 
}


void FeatureBank::_regist(const std::string& className, CreateFunc createFunc) {
        registed_features[className] = createFunc;
    }


std::unique_ptr<FeatureExtractor> FeatureBank::_create(const std::string& className) {
        auto it = registed_features.find(className);
        if (it != registed_features.end()) {
            return it->second();
        }

        throw std::runtime_error("Feature |" + className + "| not found, please check your spelling");
        // return nullptr;
    }