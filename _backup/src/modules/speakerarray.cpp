#include "speakerarray.h"
using namespace zerr;

Speaker::Speaker(int idx, float x, float y, float z, float directness, std::vector<int> contiguous)
{
  this->idx = idx;
  this->x = x;
  this->y = y;
  this->z = z;
  this->directness = directness;
  this->contiguous = contiguous;
}

void Speaker::print(){
    
    std::cout << "Speaker ID: " << idx << "\n"; 
    std::cout << "\tx,y,z = "<<x<<' '<<y<<' '<<z<<"\n"; 
    std::cout << "\tdirectness = " << directness << "\n"; 
    std::cout << "\tcontiguous = "; 
    for (int i : contiguous)
    {
        std::cout << i << ' '; 
    }
    std::cout << "\n"; 
}

void SpeakerArray::initialize(std::string config_path){
    read_config(config_path);

    YAML::Node speakerNodes = speaker_config["speakers"];
    int cnt = 0;
    for (auto it = speakerNodes.begin(); it != speakerNodes.end(); ++it){

        YAML::Node key   = it->first;
        YAML::Node value = it->second;

        int index = key.as<int>();
        float x = value['x'].as<double>();
        float y = value['y'].as<double>();
        float z = value['z'].as<double>();
        float directness = value["directness"].as<double>();
        std::vector<int> contiguous = value["connected"].as<std::vector<int>>();

        // Setup Speaker structure
        Speaker s(index, x, y, z, directness, contiguous);
        s.print();
        indexs.push_back(index);
        speakers.insert({index, s});
        //update counter
        cnt++;
    }

    n_speakers = cnt;
}

int SpeakerArray::get_n_speakers() {
    return n_speakers;
}

Speaker SpeakerArray::get_speaker_by_index(int spkr_idx){
    auto it = speakers.find(spkr_idx);

    if (it == speakers.end()) {
        throw std::out_of_range("Index not found in SpeakerArray");
    }
    return it->second;

}

std::vector<int> SpeakerArray::get_random_indexs(int l, int n){
    //TODO: remove duplicate
    assert(l >= n); 
    int idx;
    std::vector<int> random_indexs;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, l - 1);

    for (int i = 0; i < n; ++i) {
        idx = dis(gen);
        random_indexs.push_back(idx);
    }

    return random_indexs;
}

std::vector<int> SpeakerArray::get_random_speakers(int spkr_idx, int num){
    // get from all speakers, 0 is the virtual point that connects every speaker
    std::vector<int> chosn_idxs;
    if (spkr_idx == 0){ 
        int len = get_n_speakers();
        std::vector<int> arry_idxs = get_random_indexs(len, num);

        for (int i : arry_idxs){
            chosn_idxs.push_back(indexs[i]);
        }
    }
    else{
        std::vector<int> cntg_idxs = get_contiguous_speakers(spkr_idx);
        int len = cntg_idxs.size();
        std::vector<int> arry_idxs = get_random_indexs(len, num);

        for (int i : arry_idxs){
            chosn_idxs.push_back(cntg_idxs[i]);
        }
    }
    return chosn_idxs;
}

std::vector<int> SpeakerArray::get_contiguous_speakers(int spkr_idx){
    Speaker s = get_speaker_by_index(spkr_idx);
    return s.get_contiguous();
}

int SpeakerArray::get_next_one_speaker(int spkr_idx, int mode){
    int nxt_spker_idx = 0; // return the virtual point index
    std::vector<int> tmp;
    switch(mode) {
        case 0: //"RANDOM"
            tmp = get_random_speakers(spkr_idx, 1); //TODO: return int when only one speaker requested
            nxt_spker_idx = tmp[0];
            break;
        case 1: //"MONO"
            tmp = get_contiguous_speakers(spkr_idx);
            // assert(tmp.size()==1); // commented for testing
            nxt_spker_idx = tmp[0];
            break;
        case 2: //"NEAREST"
            nxt_spker_idx = 1; //TODO
            break;
        default:
            throw std::invalid_argument( "unknow speaker selecting mode.");
    }
    return nxt_spker_idx;
}

void SpeakerArray::read_config(std::string config_path){
    std::cout << "Reading speaker config!" << std::endl;

    // clear existing data
    speakers.clear();
    speaker_config = YAML::LoadFile(config_path);
}