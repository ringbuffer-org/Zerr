#include "speakermanager.h"
using namespace zerr;

/* Speaker Class Implementations */
Speaker::Speaker(t_index index, t_position position, t_orientation orientation){
    logger = new Logger();

    #ifdef TESTMODE
    logger->setLogLevel(LogLevel::INFO);
    #endif //TESTMODE

    this->index       = index;
    this->position    = position;
    this->orientation = orientation;
}


void Speaker::print_all(){
    logger->logInfo("---------------------");
    _print_index();
    _print_position();
    _print_orientation();
}


void Speaker::_print_index(){
    // std::string formated = formatString("Speaker ID: %d", index);
    // logger->logInfo("formated");
    
    std::cout << "Speaker ID: " << index << "\n"; 
}


void Speaker::_print_position(){
    std::cout << "Cartesian Position: " << "\n"; 
    std::cout  << "\t"<< "x: " << position.cartesian.x << "\n"; 
    std::cout  << "\t"<< "y: " << position.cartesian.y << "\n";
    std::cout  << "\t"<< "z: " << position.cartesian.z << "\n";

    std::cout << "Spherical Position: " << "\n"; 
    std::cout  << "\t"<< "azimuth:   " << position.spherical.azimuth << "\n"; 
    std::cout  << "\t"<< "elevation: " << position.spherical.elevation << "\n";
    std::cout  << "\t"<< "distance:  " << position.spherical.distance << "\n";
}


void Speaker::_print_orientation(){
    std::cout << "Orientation: " << "\n"; 
    std::cout  << "\t"<< "yaw:   " << orientation.yaw  << "\n";
    std::cout  << "\t"<< "pitch: " << orientation.pitch  << "\n";
}




/* SpeakerManager Class Implementations */
SpeakerManager::SpeakerManager(std::string config_path){
    this->config_path = config_path;

    logger = new Logger();

    #ifdef TESTMODE
    logger->setLogLevel(LogLevel::INFO);
    #endif //TESTMODE
    logger->logInfo("SpeakerManager::SpeakerManager logger initialized");
    logger->logInfo("SpeakerManager::SpeakerManager " + config_path);
}


bool SpeakerManager::initialize(){
    speakers.clear();
    // load speaker configuration file
    try {
        speaker_config = YAML::LoadFile(config_path);
    }
    catch (...) {
        logger->logError("SpeakerManager::initialize: Load speaker configuration " + config_path +" failed");
        return false;
    }
    // analysis speaker configuration file
    // load standard configuration of each speakers
    YAML::Node speakerNodes = speaker_config["standard"];

    for (auto it = speakerNodes.begin(); it != speakerNodes.end(); ++it){

        YAML::Node key   = it->first;
        YAML::Node value = it->second;

        int index = key.as<int>(); // assign the key to speaker index

        
        t_cartesian cartesian; // parse the position values in cartesian coordinate
        t_spherical spherical; // parse the position values in spherical coordinate
        
        if (value["position"]["cartesian"]) {
            logger->logDebug("SpeakerManager::initialize cartesian in use");
            // value["position"]["cartesian"]["x"].as<float>();
            cartesian.x = value["position"]["cartesian"]["x"].as<t_value>();
            cartesian.y = value["position"]["cartesian"]["y"].as<t_value>();
            cartesian.z = value["position"]["cartesian"]["z"].as<t_value>();
        }

        // t_cartesian cartesian; // parse the position values in cartesian coordinate
        // cartesian.x = value["position"]["cartesian"]["x"].as<t_value>();
        // cartesian.y = value["position"]["cartesian"]["y"].as<t_value>();
        // cartesian.z = value["position"]["cartesian"]["z"].as<t_value>();

        // t_spherical spherical; // parse the position values in spherical coordinate
        // spherical.azimuth   = value["position"]["spherical"]["azimuth"].as<t_value>();
        // spherical.elevation = value["position"]["spherical"]["elevation"].as<t_value>();
        // spherical.distance  = value["position"]["spherical"]["distance"].as<t_value>();

        // // check if the cartesian coordinate in origin(not assigned)
        // bool is_zero_cartesian = (cartesian.x==0.0 
        //                        && cartesian.y==0.0 
        //                        && cartesian.z==0.0); 

        // // check if the spherical coordinate in origin(not assigned)
        // bool is_zero_spherical = (spherical.azimuth  ==0.0 
        //                        && spherical.elevation==0.0 
        //                        && spherical.distance ==0.0); 
        // // both cartesian and spherical in origin is not allowed
        // assert(!(is_zero_cartesian && is_zero_spherical));     
        // // check if the given two types of coordinate the same
        // // if (is_zero_cartesian && is_zero_spherical) {check_coordinate_consistency(cartesian, spherical)} 

        // #ifdef TESTMODE
        // // std::cout<<"is_zero_spherical: "<<is_zero_spherical<<std::endl;
        // // std::cout<<"is_zero_cartesian: "<<is_zero_cartesian<<std::endl;
        // #endif //TESTMODE

        // cartesian = is_zero_cartesian?_spherical2cartesian(spherical):cartesian; // fill the cartesian if it's not assigned
        // spherical = is_zero_spherical?_cartesian2spherical(cartesian):spherical; // fill the spherical if it's not assigned

        // t_position position;
        // position.cartesian = cartesian;
        // position.spherical = spherical;

        // t_orientation orientation;
        // orientation.yaw   = value["orientation"]["yaw"].as<t_value>();
        // orientation.pitch = value["orientation"]["pitch"].as<t_value>();

        // // Setup Speaker structure
        // Speaker s(index, position, orientation);

        // #ifdef TESTMODE
        // s.print_all();
        // #endif //TESTMODE

        // // indexs.push_back(index);
        // speakers.insert({index, s});

        // cnt++;
    }

    // n_speakers = cnt;

    //load specific configuration of the active sub speaker array
    // YAML::Node specificNodes = speaker_config["specific"][mode];
    // assert(specificNodes["unmasked"].IsSequence() && "SpeakerManager::initialize: unmasked property must be a list");
    // assert(specificNodes["subindex"].IsSequence() && "SpeakerManager::initialize: subindex property must be a list");

    // for (size_t i = 0; i < specificNodes["unmasked"].size(); ++i) {
    //     unmasked.push_back(specificNodes["unmasked"][i].as<t_index>());
    // }
    // _init_distance_matrix();

    // for (size_t i = 0; i < specificNodes["subindex"].size(); ++i) {
    //     subindex.push_back(specificNodes["subindex"][i].as<t_index>());
    // }

    // // topology_matrix
    // YAML::Node topologyNode = specificNodes["topology"];
    // for (auto it = topologyNode.begin(); it != topologyNode.end(); ++it){
    //     YAML::Node key   = it->first;
    //     YAML::Node value = it->second;

    //     t_indexs tmp_idx;
    //     assert(value.IsSequence() && "SpeakerManager::initialize: topology property must be a list");
    //     for (size_t i = 0; i < value.size(); ++i) {
    //         tmp_idx.push_back(value[i].as<t_index>());
    //     } 
    //     topology_matrix[key.as<t_index>()] = tmp_idx;
    // }

    // #ifdef TESTMODE
    // std::cout << "---------------------" << "\n"; 
    // std::cout << "topology_matrix------" << "\n"; 
    // for (const auto& maps : topology_matrix) {
    //     t_index  key   = maps.first;
    //     t_indexs value = maps.second;
    //     std::cout << "ID: " << key << std::endl;
    //     std::cout << "  Connected: ";
    //     for (int i = 0; i < value.size(); ++i){
    //          std::cout << value[i] << " ";
    //      } 
    //     std::cout << std::endl;
    // }
    // #endif //TESTMODE

    // set the speaker manager as initialized
    initialized=true;

    return initialized;
}


size_t SpeakerManager::get_n_all_speakers() {
    // _is_initialized();
    return speakers.size();
}


size_t SpeakerManager::get_n_unmasked_speakers() {
    // _is_initialized();
    return unmasked.size();
}


Speaker SpeakerManager::get_speaker_by_index(t_index spkr_idx){
    auto it = speakers.find(spkr_idx);

    if (it == speakers.end()) {
        throw std::out_of_range("Index not found in SpeakerManager");
    }
    return it->second;
}


t_pair SpeakerManager::get_indexs_by_trajectory(t_value trajectory_val){
    // Clip the input trajectory to [0.0,1.0]
    trajectory_val = trajectory_val>1.0?1.0:trajectory_val;
    trajectory_val = trajectory_val<0.0?0.0:trajectory_val;

    t_value scaled = trajectory_val * (subindex.size() - 1);

    t_index lower = subindex[std::floor(scaled)];
    t_index upper = subindex[std::ceil(scaled)];

    return std::make_pair(lower, upper);
}


t_pair SpeakerManager::get_indexs_by_geometry(std::vector<t_value> pos, std::vector<bool> mask, std::string coordinate){
    std::vector<t_value> distance;
    std::vector<t_index> indexs;

    assert(mask.size() == 3 && "ERROR: The mask vector for geometry selection muss be in size 3.");

    t_value tmp_distan;
    for (const auto& maps : speakers) {
        t_index key  = maps.first;
        Speaker spkr = maps.second;
        
        if (coordinate == "cartesian"){
            tmp_distan = abs(spkr.get_x() - pos[0])*mask[0] + 
                         abs(spkr.get_y() - pos[1])*mask[1] +
                         abs(spkr.get_z() - pos[2])*mask[2];

        }else{ // spherical TODO: make them in the same weight
            tmp_distan = abs(spkr.get_azimuth() - pos[0])*mask[0] + 
                         abs(spkr.get_elevation() - pos[1])*mask[1] +
                         abs(spkr.get_distance() - pos[2])*mask[2];
        }
        distance.push_back(tmp_distan);
        indexs.push_back(key);
    }

    size_t first = 0;
    size_t second = 1;

    for (size_t i = 2; i < distance.size(); ++i) {
        if (distance[i] < distance[first]) {
            second = first;
            first = i;
        } else if (distance[i] < distance[second]) {
            second = i;
        }
    }

    t_index smallest = get_speaker_by_index(indexs[first]).get_index();
    t_index second_small = get_speaker_by_index(indexs[second]).get_index();

    return std::make_pair(smallest, second_small);
}


t_index SpeakerManager::get_indexs_by_trigger(t_value trigger, t_index curr_spkr, int mode){

    assert(trigger<0.01 || trigger>0.99);
    // not a positive trigger, just return the original one
    if (trigger<0.01) return curr_spkr;
    // load all connected speakers from the topology matrix
    t_indexs candidates = topology_matrix[curr_spkr];
    // if only one speaker connected to the current one, just return it
    if (candidates.size()==1) return candidates[0];

    t_index selected;
    int n_candidates = candidates.size();
    switch(mode){
        case 0: //"Random"
            selected = candidates[_get_random_indexs(n_candidates, 1)[0]];
            break;
        case 1: //"Nearest"
            selected = _find_nearest(curr_spkr, candidates);
            break;
        case 2: //"All"
            selected = candidates[0];
            break;
        default:
            throw std::invalid_argument( "Unknow speaker selecting mode.");
    }

    return selected;
}


std::vector<t_value> SpeakerManager::get_distance_vector(int spkr_idx){
    return distance_matrix[spkr_idx-1]; // speaker index starts from 1
}


void SpeakerManager::_is_initialized() {
    assert(initialized==true 
        && "ERROR: SpeakerManager isn't initialized");
}


void SpeakerManager::_init_distance_matrix(){
    int n_speakers = get_n_unmasked_speakers();
    // distance_matrix.resize(n_speakers);
    // for (auto& vec : distance_matrix) {
    //     vec.resize(n_speakers);
    // }

    for (int i = 0; i < n_speakers; ++i){
        distance_matrix[unmasked[i]] = {};
        for (int j = 0; j < n_speakers; ++j){
            Speaker s1 = get_speaker_by_index(unmasked[i]); // speaker index starts from 1
            Speaker s2 = get_speaker_by_index(unmasked[j]); // speaker index starts from 1
            distance_matrix[unmasked[i]].push_back(_calculate_distance(s1, s2));
        }
    }
}


t_index SpeakerManager::_find_nearest(t_index curr_spkr, t_indexs candidates){
    //TODO: seems incorrect
    t_value min_distance = distance_matrix[curr_spkr][0];
    t_index nearest = candidates[0];
    t_value tmp_distance;
    for (size_t i = 1; i < candidates.size(); ++i){
        tmp_distance = distance_matrix[curr_spkr][i];
        if (min_distance > tmp_distance){
            nearest = candidates[i];
            min_distance = tmp_distance;
        }
    }

    return nearest;
}


float SpeakerManager::_calculate_distance(Speaker s1, Speaker s2){
    float dx = s1.get_x() - s2.get_x();
    float dy = s1.get_y() - s2.get_y();
    float dz = s1.get_z() - s2.get_z();

    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

    return distance;
}


std::vector<int> SpeakerManager::_get_random_indexs(int l, int n){
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


t_cartesian SpeakerManager::_spherical2cartesian(t_spherical spherical){
    //test 
    t_cartesian cartesian;
    cartesian.x = spherical.distance * cos(spherical.elevation/180.0*PI) * cos(spherical.azimuth/180.0*PI);
    cartesian.y = spherical.distance * cos(spherical.elevation/180.0*PI) * sin(spherical.azimuth/180.0*PI);
    cartesian.z = spherical.distance * sin(spherical.elevation/180.0*PI);

    return cartesian;
}

t_spherical SpeakerManager::_cartesian2spherical(t_cartesian cartesian){
    t_spherical spherical;
    spherical.distance  = sqrt(cartesian.x * cartesian.x + cartesian.y * cartesian.y + cartesian.z * cartesian.z);
    spherical.azimuth   = atan2(cartesian.y, cartesian.x) / PI * 180.0;
    spherical.elevation = asin(cartesian.z / spherical.distance) / PI * 180.0;

    return spherical;
}

