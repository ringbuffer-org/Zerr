#include "speakermanager.h"
using namespace zerr;


Speaker::Speaker(Index index, Position position, Orientation orientation){
    logger = new Logger();

    #ifdef TESTMODE
    logger->setLogLevel(LogLevel::INFO);
    #endif //TESTMODE

    this->index       = index;
    this->position    = position;
    this->orientation = orientation;
}


void Speaker::print_all(){
    logger->logInfo("-----------------------");
    _print_index();
    _print_position();
    _print_orientation();
}


void Speaker::_print_index(){
    logger->logInfo(formatString("Speaker ID: %d", index));
}


void Speaker::_print_position(){
    logger->logInfo("Cartesian Position: ");
    logger->logInfo(formatString("    x: %.2f", position.cartesian.x));
    logger->logInfo(formatString("    y: %.2f", position.cartesian.y));
    logger->logInfo(formatString("    z: %.2f", position.cartesian.z));
    logger->logInfo("Spherical Position: ");
    logger->logInfo(formatString("    azimuth:   : %.2f", position.spherical.azimuth));
    logger->logInfo(formatString("    elevation: : %.2f", position.spherical.elevation));
    logger->logInfo(formatString("    distance:  : %.2f", position.spherical.distance));
}


void Speaker::_print_orientation(){
    logger->logInfo("Orientation: ");
    logger->logInfo(formatString("    yaw:   : %.2f", orientation.yaw));
    logger->logInfo(formatString("    pitch: : %.2f", orientation.pitch));
}




/* SpeakerManager Class Implementations */
SpeakerManager::SpeakerManager(std::string config_path){
    this->config_path = config_path;

    logger = new Logger();

    #ifdef TESTMODE
    logger->setLogLevel(LogLevel::INFO);
    #endif //TESTMODE
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

        Cartesian cartesian; // parse the position values in cartesian coordinate
        Spherical spherical; // parse the position values in spherical coordinate

        bool is_zero_cartesian = true;
        bool is_zero_spherical = true;

        if (value["position"]["cartesian"] && 
                !value["position"]["cartesian"].IsNull()) {
            cartesian.x = value["position"]["cartesian"]["x"].as<Param>();
            cartesian.y = value["position"]["cartesian"]["y"].as<Param>();
            cartesian.z = value["position"]["cartesian"]["z"].as<Param>();
            is_zero_cartesian = false;
        }

        if (value["position"]["spherical"] && 
                !value["position"]["spherical"].IsNull()) {
            spherical.azimuth   = value["position"]["spherical"]["azimuth"].as<Param>();
            spherical.elevation = value["position"]["spherical"]["elevation"].as<Param>();
            spherical.distance  = value["position"]["spherical"]["distance"].as<Param>();
            is_zero_spherical = false;
        }

        // both cartesian and spherical in origin is not allowed
        assert(!(is_zero_cartesian && is_zero_spherical));
        // TODO: check if the given two types of coordinate the same
        // if (is_zero_cartesian && is_zero_spherical) {check_coordinate_consistency(cartesian, spherical)} 

        cartesian = is_zero_cartesian?_spherical2cartesian(spherical):cartesian; // fill the cartesian if it's not assigned
        spherical = is_zero_spherical?_cartesian2spherical(cartesian):spherical; // fill the spherical if it's not assigned

        Position            position;
        position.cartesian = cartesian;
        position.spherical = spherical;

        Orientation orientation;
        if (value["orientation"] && 
                !value["orientation"].IsNull()) {
            orientation.yaw   = value["orientation"]["yaw"].as<Param>();
            orientation.pitch = value["orientation"]["pitch"].as<Param>();
        }else { // TODO: keep the orientation structure
            orientation.yaw = 0.0;
            orientation.pitch = 0.0;
        }

        // Setup Speaker structure
        Speaker s(index, position, orientation);

        #ifdef TESTMODE
        s.print_all();
        #endif //TESTMODE

        speakers.insert({index, s});

        actvSpkIdx.push_back(index);
    }
    _init_distance_matrix();

    // initialize the specific configs
    // every speaker is actvSpkIdx when at initialize point
    for (size_t i = 0; i < actvSpkIdx.size(); ++i){
        trajVector.push_back(actvSpkIdx[i]);
    }
    std::sort(trajVector.begin(), trajVector.end());

    // topoMatrix every speaker is connected
    for (size_t i = 0; i < actvSpkIdx.size(); ++i){
        Indexes tmp_idx = actvSpkIdx; // deepcopy
        topoMatrix[actvSpkIdx[i]] = tmp_idx;
    }

    return true;
}


size_t SpeakerManager::getNumAllSpeakers() {
    return speakers.size();
}


size_t SpeakerManager::getNumActiveSpeakers() {
    return actvSpkIdx.size();
}


Indexes SpeakerManager::getActiveSpeakerIndexs(){
    return actvSpkIdx;
}


Index SpeakerManager::get_random_index(){
    return actvSpkIdx[_get_random_indexs(actvSpkIdx.size(), 1)[0]];
}


Speaker SpeakerManager::get_speaker_by_index(Index spkr_idx){
    auto it = speakers.find(spkr_idx);

    if (it == speakers.end()) {
        throw std::out_of_range("Index not found in SpeakerManager");
    }
    return it->second;
}


Pair SpeakerManager::get_indexs_by_trajectory(Param trajectory_val){
    // Clip the input trajectory to [0.0,1.0]
    trajectory_val = trajectory_val>1.0?1.0:trajectory_val;
    trajectory_val = trajectory_val<0.0?0.0:trajectory_val;

    Param scaled = trajectory_val * (trajVector.size() - 1);

    Index lower = trajVector[std::floor(scaled)];
    Index upper = trajVector[std::ceil(scaled)];

    return std::make_pair(lower, upper);
}


Param SpeakerManager::get_panning_ratio(Param trajectory_val){
    trajectory_val = trajectory_val>1.0?1.0:trajectory_val;
    trajectory_val = trajectory_val<0.0?0.0:trajectory_val;

    // trajectory_val = trajectory_val;

    Param scaled = trajectory_val * (trajVector.size() - 1);

    return (scaled - std::floor(scaled)) / (std::ceil(scaled) - std::floor(scaled));
}


Pair SpeakerManager::get_indexs_by_geometry(std::vector<Param> pos, std::vector<bool> mask, std::string coordinate){
    std::vector<Param> distance;
    std::vector<Index> indexs;

    assert(mask.size() == 3 && "ERROR: The mask vector for geometry selection muss be in size 3.");

    Param tmp_distan;
    for (const auto& maps : speakers) {
        Index key  = maps.first;
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

    Index smallest = get_speaker_by_index(indexs[first]).get_index();
    Index second_small = get_speaker_by_index(indexs[second]).get_index();

    return std::make_pair(smallest, second_small);
}


Index SpeakerManager::get_indexs_by_trigger(Param trigger, Index curr_spkr, Mode mode){
    // just return the original one when trigger doesn't close to 1.0
    if (!isEqualTo1(trigger, TRIGGER_THRESHOLD)) return curr_spkr;
    // load all connected speakers from the topology matrix
    Indexes candidates = topoMatrix[curr_spkr];
    // if only one speaker connected to the current one, just return it
    if (candidates.size()==1) return candidates[0];

    Index selected;
    int n_candidates = candidates.size();

    selected = candidates[_get_random_indexs(n_candidates, 1)[0]];
    // switch(mode){
    //     case 0: // "Random"
    //         selected = candidates[_get_random_indexs(n_candidates, 1)[0]];
    //         break;
    //     case 1: // "Nearest"
    //         selected = _find_nearest(curr_spkr, candidates);
    //         break;
    //     default:
    //         throw std::invalid_argument( "Unknow trigger mode.");
    // }

    return selected;
}


std::vector<Param> SpeakerManager::get_distance_vector(int spkr_idx){
    return distance_matrix[spkr_idx]; 
}


void SpeakerManager::setActiveSpeakers(std::string action, Indexes idxs){
    if (action=="set"){
        _setActiveSpeakerIndexs(idxs);
    }
    else if (action=="add"){
        _addActiveSpeakerIndexs(idxs);
    }
    else if (action=="del"){
        _delActiveSpeakerIndexs(idxs);
    }
    else{
        logger->logWarning("SpeakerManager::setActiveSpeakers unknown action " + action);
    }

    #ifdef TESTMODE
    printActiveSpeakerIndexs();
    #endif  // TESTMODE
}


void SpeakerManager::setTrajectoryVector(Indexes idxs){
    Indexes tmpTrajVector;
    for (size_t i = 0; i < idxs.size(); ++i){
        if(!isInVec<Index>(idxs[i], actvSpkIdx)) {
            logger->logError(formatString("SpeakerManager: speaker %d is not activated!", idxs[i]));
            return;
        }else{
            tmpTrajVector.push_back(idxs[i]);
        }
    }
    trajVector = tmpTrajVector;
    #ifdef TESTMODE
    printTrajectoryVector();
    #endif  // TESTMODE
}

void SpeakerManager::setTopoMatrix(std::string action, Indexes idxs) {
    if (action=="set") {
        _setTopoMatrixIndexes(idxs);
    }
    else if (action=="add"){
        _addTopoMatrixIndexes(idxs);
    }
    else if (action=="del"){
        _delTopoMatrixIndexes(idxs);
    }
    else{
        logger->logWarning("SpeakerManager::setTopoMatrix unknown action " + action);
    }
    #ifdef TESTMODE
    printTopoMatrix();
    #endif //TESTMODE
}

bool SpeakerManager::_isActivated(Index idx) {
    if(!isInVec<Index>(idx, actvSpkIdx)) {
        logger->logError(formatString("Speaker %d is not activated!", idx));
        return false;
    }
    return true;
}


void SpeakerManager::_setTopoMatrixIndexes(Indexes idxs) {
    Index mainSpkr = idxs[0];
    if (!_isActivated(mainSpkr)) return;

    Indexes tmpTopoVector;
    for (size_t i = 1; i < idxs.size(); ++i){
        if (!_isActivated(idxs[i])) return;
        tmpTopoVector.push_back(idxs[i]);
    }
    topoMatrix[mainSpkr] = tmpTopoVector;
}

void SpeakerManager::_addTopoMatrixIndexes(Indexes idxs) {
    Index mainSpkr = idxs[0];
    if (!_isActivated(mainSpkr)) return;

    for (size_t i = 1; i < idxs.size(); ++i){
        if (!_isActivated(idxs[i])) continue;
        if (isInVec<Index>(idxs[i], topoMatrix[mainSpkr])) continue;
        topoMatrix[mainSpkr].push_back(idxs[i]);
    }
}

void SpeakerManager::_delTopoMatrixIndexes(Indexes idxs) {
    Index mainSpkr = idxs[0];
    Indexes& connected = topoMatrix[mainSpkr];

    if (!_isActivated(mainSpkr)) return;
    for (size_t i = 1; i < idxs.size(); ++i){
        if (!_isActivated(idxs[i])) continue;
        if (!isInVec<Index>(idxs[i], connected)) continue;

        connected.erase(std::remove(connected.begin(), connected.end(), idxs[i]), connected.end());
    }
}

void SpeakerManager::printParameters() {
    printActiveSpeakerIndexs();
    printTrajectoryVector();
    printTopoMatrix();
}

void SpeakerManager::printActiveSpeakerIndexs(){
    logger->logInfo("Active Speakers: ");
    logger->logInfo("    " + formatVector<Index>(actvSpkIdx));
}


void SpeakerManager::printTopoMatrix(){
    logger->logInfo("Topological Matrix: ");
    for (auto it = topoMatrix.begin(); it != topoMatrix.end(); ++it) {
        logger->logInfo("    " + std::to_string(it->first) + " | " + formatVector<Index>(it->second));
    }
}


void SpeakerManager::printTrajectoryVector(){
    logger->logInfo("Trajectory Vector: ");
    logger->logInfo("    " + formatVector<Index>(trajVector));
}


void SpeakerManager::_init_distance_matrix(){
    int n_speakers = getNumActiveSpeakers();
    for (int i = 0; i < n_speakers; ++i){
        distance_matrix[actvSpkIdx[i]] = {};
        for (int j = 0; j < n_speakers; ++j){
            Speaker s1 = get_speaker_by_index(actvSpkIdx[i]);  
            Speaker s2 = get_speaker_by_index(actvSpkIdx[j]);  
            distance_matrix[actvSpkIdx[i]].push_back(_calculate_distance(s1, s2));
        }
    }
}


Index SpeakerManager::_find_nearest(Index curr_spkr, Indexes candidates){
    //TODO: seems incorrect
    Param min_distance = distance_matrix[curr_spkr][0];
    Index nearest = candidates[0];
    Param tmp_distance;
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


Cartesian SpeakerManager::_spherical2cartesian(Spherical spherical){
    //test 
    Cartesian cartesian;
    cartesian.x = spherical.distance * cos(spherical.elevation/180.0*PI) * cos(spherical.azimuth/180.0*PI);
    cartesian.y = spherical.distance * cos(spherical.elevation/180.0*PI) * sin(spherical.azimuth/180.0*PI);
    cartesian.z = spherical.distance * sin(spherical.elevation/180.0*PI);

    return cartesian;
}


Spherical SpeakerManager::_cartesian2spherical(Cartesian cartesian){
    Spherical spherical;
    spherical.distance  = sqrt(cartesian.x * cartesian.x + cartesian.y * cartesian.y + cartesian.z * cartesian.z);
    spherical.azimuth   = atan2(cartesian.y, cartesian.x) / PI * 180.0;
    spherical.elevation = asin(cartesian.z / spherical.distance) / PI * 180.0;

    return spherical;
}


void SpeakerManager::_setActiveSpeakerIndexs(Indexes idxs){
    actvSpkIdx.clear();
    for (size_t i = 0; i < idxs.size(); ++i){
        auto it = speakers.find(idxs[i]);
        if(it == speakers.end()) {
            logger->logError(formatString("SpeakerManager::_set_actvSpkIdx_indexs unknow speaker index %d!", idxs[i]));
            return;
        }        
        // add to actvSpkIdx
        if(isInVec<Index>(idxs[i], actvSpkIdx)) {
            logger->logWarning(formatString("SpeakerManager: index %d already added, ignored", idxs[i]));
        }else{
            actvSpkIdx.push_back(idxs[i]);
        }
    }
    // reset also distance matrix and topoMatrix to init state
    distance_matrix.clear();
    _init_distance_matrix();

    trajVector.clear();
    for (size_t i = 0; i < actvSpkIdx.size(); ++i){
        trajVector.push_back(actvSpkIdx[i]);
    }
    std::sort(trajVector.begin(), trajVector.end()); 

    topoMatrix.clear();
    for (size_t i = 0; i < actvSpkIdx.size(); ++i){
        Indexes tmp_idx = actvSpkIdx;  
        topoMatrix[actvSpkIdx[i]] = tmp_idx;
    }
}


void SpeakerManager::_addActiveSpeakerIndexs(Indexes idxs){
    for (size_t i = 0; i < idxs.size(); ++i){
        // check if the input index is valid
        if(!isInKey<Index, Speaker>(idxs[i], speakers)){
            logger->logError("SpeakerManager unknow speaker index!");
            return;
        }
        // add to actvSpkIdx
        if(isInVec<Index>(idxs[i], actvSpkIdx)) {
            logger->logWarning(formatString("SpeakerManager: index %d already added, ignored", idxs[i]));
        }else{
            actvSpkIdx.push_back(idxs[i]);
        }
    }
}


void SpeakerManager::_delActiveSpeakerIndexs(Indexes idxs){
    for (size_t i = 0; i < idxs.size(); ++i){
        // check if the input index is valid
        if(!isInKey<Index, Speaker>(idxs[i], speakers)){
            logger->logError("SpeakerManager unknow speaker index!");
            return;
        }
        // remove from actvSpkIdx
        if(!isInVec<Index>(idxs[i], actvSpkIdx)) {
            logger->logWarning(formatString("SpeakerManager index %d already removed, ignored", idxs[i]));
        }else{
            actvSpkIdx.erase(std::remove(actvSpkIdx.begin(), actvSpkIdx.end(), idxs[i]), actvSpkIdx.end());
        }
        // remove from trajVector
        if(!isInVec<Index>(idxs[i], trajVector)) {
            logger->logWarning(formatString("SpeakerManager index %d already removed, ignored", idxs[i]));
        }else{
            trajVector.erase(std::remove(trajVector.begin(), trajVector.end(), idxs[i]), trajVector.end());
        }
        // remove from topology matrix
        if (isInKey<Index, Indexes>(idxs[i], topoMatrix)) {
            topoMatrix.erase(idxs[i]);
        } 
        for (auto it = topoMatrix.begin(); it != topoMatrix.end(); ++it) {
            if(isInVec<Index>(idxs[i], it->second)) {
                it->second.erase(std::remove(it->second.begin(), it->second.end(), idxs[i]), it->second.end());
            }
        }
    }
}