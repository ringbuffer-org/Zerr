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
    speakers.clear();
    speaker_config = YAML::LoadFile(config_path);


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
        // s.print();
        indexs.push_back(index);
        speakers.insert({index, s});

        cnt++;
    }

    n_speakers = cnt;

    _init_distance_matrix();
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


std::vector<int> SpeakerArray::get_random_speakers(int spkr_idx, int num){
    // get from all speakers, 0 is the virtual point that connects every speaker
    std::vector<int> chosn_idxs;
    if (spkr_idx == 0){ 
        int len = get_n_speakers();
        std::vector<int> arry_idxs = _get_random_indexs(len, num);

        for (int i : arry_idxs){
            chosn_idxs.push_back(indexs[i]);
        }
    }
    else{
        std::vector<int> cntg_idxs = get_contiguous_speakers(spkr_idx);
        int len = cntg_idxs.size();
        std::vector<int> arry_idxs = _get_random_indexs(len, num);

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


std::vector<float> SpeakerArray::get_distance_vector(int spkr_idx){
    return distance_matrix[spkr_idx-1]; // speaker index starts from 1
}


void SpeakerArray::_init_distance_matrix(){
    distance_matrix.resize(n_speakers);
    for (auto& vec : distance_matrix) {
        vec.resize(n_speakers);
    }

    //computation inefficient, but this will only called once :(
    for (int i = 0; i < n_speakers; ++i){
        for (int j = 0; j < n_speakers; ++j){
            Speaker s1 = get_speaker_by_index(i+1); // speaker index starts from 1
            Speaker s2 = get_speaker_by_index(j+1); // speaker index starts from 1
            distance_matrix[i][j] = _calculate_distance(s1, s2);
        }
    }

    #ifdef   TESTMODE
    // print_mat(distance_matrix);
    #endif //TESTMODE
}


float SpeakerArray::_calculate_distance(Speaker s1, Speaker s2){
    float dx = s1.get_x() - s2.get_x();
    float dy = s1.get_y() - s2.get_y();
    float dz = s1.get_z() - s2.get_z();

    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

    return distance;
}


std::vector<int> SpeakerArray::_get_random_indexs(int l, int n){
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


// remove duplicate version from ChatGPT, not checked
// std::vector<int> SpeakerArray::_get_random_indexs(int l, int n) {
//     assert(l >= n);  // Assert that the range is greater than or equal to the number of indices needed

//     std::vector<int> random_indexs;  // Vector to store the random indices
//     std::unordered_set<int> generated_indexs;  // Set to keep track of generated indices

//     std::random_device rd;  // Seed for the random number generator
//     std::mt19937 gen(rd());  // Mersenne Twister random number engine
//     std::uniform_int_distribution<> dis(0, l - 1);  // Uniform distribution of integers between 0 and l-1

//     while (random_indexs.size() < n) {
//         int idx = dis(gen);  // Generate a random index using the distribution and random number generator

//         // Check if the generated index is already in the set
//         if (generated_indexs.find(idx) == generated_indexs.end()) {
//             generated_indexs.insert(idx);  // Add the index to the set
//             random_indexs.push_back(idx);  // Add the index to the vector
//         }
//     }

//     return random_indexs;  // Return the vector of random indices
// }
