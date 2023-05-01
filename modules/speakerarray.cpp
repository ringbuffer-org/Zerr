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

int SpeakerArray::get_random_one_speaker(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, indexs.size() - 1);
    int i = dis(gen);
    
    return indexs[i];
}

// std::vector<int> SpeakerArray::get_random_speakers(int n){

// }

// int SpeakerArray::get_next_one_speaker(int spkr_idx, std::string mode){

// }

// std::vector<int> SpeakerArray::get_connected_speakers(int spkr_idx){

// }

void SpeakerArray::read_config(std::string config_path){
    std::cout << "Reading speaker config!" << std::endl;

    // clear existing data
    speakers.clear();
    speaker_config = YAML::LoadFile(config_path);

}






// void SpeakerArray::init_contiguous_matrix(){
    // }