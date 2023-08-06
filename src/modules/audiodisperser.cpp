#include "audiodisperser.h"
using namespace zerr;


AudioDisperser::AudioDisperser(int numChannel, zerr::t_systemConfigs systemCfgs){
    this->numChannel = numChannel;
    this->systemCfgs = systemCfgs;

    numInlet  = numChannel+1;
    numOutlet = numChannel;

    logger = new Logger();
    #ifdef TESTMODE
    logger->setLogLevel(LogLevel::INFO);
    #endif //TESTMODE
}

bool AudioDisperser::initialize(){
    inputBuffer.resize(numInlet,   t_samples(systemCfgs.block_size, 0.0f));
    outputBuffer.resize(numOutlet, t_samples(systemCfgs.block_size, 0.0f));

    return true;
}


void AudioDisperser::fetch(t_blockIns in){
    inputBuffer = in;
}


void AudioDisperser::process(){
    // clean the output buffer
    for (auto& buffer : outputBuffer) {
        buffer.assign(buffer.size(), 0.0f);
    }

    t_samples& source =  inputBuffer[0];

    for (int i = 1; i < numInlet; ++i) {
        for (int j = 0; j < systemCfgs.block_size; ++j) {
            outputBuffer[i-1][j] = source[j] * inputBuffer[i][j];
        }
    }

}


// void AudioDisperser::_process_add(){
//     // clean the output buffer
//     for (auto& buffer : outputBuffer) {
//         buffer.assign(buffer.size(), 0.0f);
//     }

//     for (int i = 0; i < numChannel; ++i) {
//         for (int j = 0; j < numSource; ++j) {
//             for (int k = 0; k < systemCfgs.block_size; ++k) {
//                 outputBuffer[i][k] += inputBuffer[i + j*numChannel][k];
//             }
//         }
//     }
// }


// void AudioDisperser::_process_root(){
//     // clean the output buffer
//     for (auto& buffer : outputBuffer) {
//         buffer.assign(buffer.size(), 0.0f);
//     }

//     double exponent = 1.0 / (double) numSource;
//     t_sample multi_tmp;
//     for (int i = 0; i < numChannel; ++i) {
//         for (int k = 0; k < systemCfgs.block_size; ++k) {
//             multi_tmp = 1;
//             for (int j = 0; j < numSource; ++j) {
//                 multi_tmp *= inputBuffer[i + j*numChannel][k];
//             }
//             outputBuffer[i][k] = std::pow(std::abs(multi_tmp), exponent);
//         }
//     }
// }


// void AudioDisperser::_process_max(){
//     // clean the output buffer
//     for (auto& buffer : outputBuffer) {
//         buffer.assign(buffer.size(), 0.0f);
//     }

//     t_sample maxVal;
//     t_sample tmp;
//     for (int i = 0; i < numChannel; ++i) {
//         for (int k = 0; k < systemCfgs.block_size; ++k) {
//             maxVal = 0;
//             for (int j = 0; j < numSource; ++j) {
//                 tmp = inputBuffer[i + j*numChannel][k];
//                 maxVal = tmp>maxVal?tmp:maxVal;
//             }
//             outputBuffer[i][k] = maxVal;
//         }
//     }
// }


t_blockOuts AudioDisperser::send(){
    return outputBuffer;
}

// #include "audiorouter.h"
// using namespace zerr;

// typedef std::vector<t_blockIn> out_mat;

// void AudioRouter::initialize(int num_audio, int num_speaker){
//     audio_x.resize(num_audio, 0.0f);
//     cntrl_x.resize(num_speaker, 0.0f);

//     y.resize(num_speaker);
//     for (auto& vec : y) {
//         vec.resize(num_audio);
//     }

//     linear_interpolator.initialize(num_speaker);
// }

// void AudioRouter::fetch(t_blockIn audio_in, t_volumes cntrl_in){

//     cntrl_x_prev = cntrl_x;

//     main_volume = cntrl_in[0];

//     audio_x = audio_in;

//     cntrl_x.assign(cntrl_in.begin() + 1, cntrl_in.end());

//     linear_interpolator.set_values(cntrl_x_prev, cntrl_x, audio_x.size());
// }

// void AudioRouter::process(){
//     t_volumes tmp;
    
//     for (int j = 0; j < audio_x.size(); ++j){

//         tmp = linear_interpolator.get_value();

//         for (int i = 0; i < cntrl_x.size(); ++i){
//             y[i][j] = main_volume * tmp[i] * audio_x[j];
//         }

//         linear_interpolator.next_step();
//     }
// }

// out_mat AudioRouter::send(){
//     return y;
// }
