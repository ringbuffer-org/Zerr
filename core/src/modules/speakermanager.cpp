/**
 * @file speakermanager.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief  Speaker & SpeakerManager Classes Implementation
 * @date 2024-02-18
 *
 * @copyright Copyright (c) 2023-2024
 *
 */
#include "speakermanager.h"
using namespace zerr;

Speaker::Speaker(Index index, Position position, Orientation orientation) {
    logger = new Logger();

#ifdef TESTMODE
    logger->setLogLevel(LogLevel::INFO);
#endif  // TESTMODE

    this->index = index;
    this->position = position;
    this->orientation = orientation;
}

void Speaker::printAll() {
    logger->logInfo("-----------------------");
    _print_index();
    _print_position();
    _print_orientation();
}

void Speaker::_print_index() {
    logger->logInfo(formatString("Speaker ID: %d", index));
}

void Speaker::_print_position() {
    logger->logInfo("Cartesian Position: ");
    logger->logInfo(formatString("    x: %.2f", position.cartesian.x));
    logger->logInfo(formatString("    y: %.2f", position.cartesian.y));
    logger->logInfo(formatString("    z: %.2f", position.cartesian.z));
    logger->logInfo("Spherical Position: ");
    logger->logInfo(
        formatString("    azimuth:   : %.2f", position.spherical.azimuth));
    logger->logInfo(
        formatString("    elevation: : %.2f", position.spherical.elevation));
    logger->logInfo(
        formatString("    distance:  : %.2f", position.spherical.distance));
}

void Speaker::_print_orientation() {
    logger->logInfo("Orientation: ");
    logger->logInfo(formatString("    yaw:   : %.2f", orientation.yaw));
    logger->logInfo(formatString("    pitch: : %.2f", orientation.pitch));
}

SpeakerManager::SpeakerManager(ConfigPath spkrArry) {
    this->speakerArrayPath = spkrArry;

    logger = new Logger();

#ifdef TESTMODE
    logger->setLogLevel(LogLevel::INFO);
#endif  // TESTMODE
    logger->logInfo("SpeakerManager::SpeakerManager " + speakerArrayPath);
}

bool SpeakerManager::initialize() {
    speakers.clear();
    // load speaker configuration file
    try {
        speakerArrayNode = YAML::LoadFile(speakerArrayPath);
    } catch (...) {
        logger->logError(
            "SpeakerManager::initialize: Load speaker configuration " +
            speakerArrayPath + " failed");
        return false;
    }
    // analysis speaker configuration file
    // load standard configuration of each speakers
    YAML::Node speakerNodes = speakerArrayNode["standard"];

    for (auto it = speakerNodes.begin(); it != speakerNodes.end(); ++it) {
        YAML::Node key = it->first;
        YAML::Node value = it->second;

        int index = key.as<int>();  // assign the key to speaker index

        Cartesian
            cartesian;  // parse the position values in cartesian coordinate
        Spherical
            spherical;  // parse the position values in spherical coordinate

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
            spherical.azimuth =
                value["position"]["spherical"]["azimuth"].as<Param>();
            spherical.elevation =
                value["position"]["spherical"]["elevation"].as<Param>();
            spherical.distance =
                value["position"]["spherical"]["distance"].as<Param>();
            is_zero_spherical = false;
        }

        // both cartesian and spherical in origin is not allowed
        assert(!(is_zero_cartesian && is_zero_spherical));
        // TODO: check if the given two types of coordinate the same
        // if (is_zero_cartesian && is_zero_spherical)
        // {check_coordinate_consistency(cartesian, spherical)}

        cartesian = is_zero_cartesian
                        ? _spherical2cartesian(spherical)
                        : cartesian;  // fill the cartesian if it's not assigned
        spherical = is_zero_spherical
                        ? _cartesian2spherical(cartesian)
                        : spherical;  // fill the spherical if it's not assigned

        Position position;
        position.cartesian = cartesian;
        position.spherical = spherical;

        Orientation orientation;
        if (value["orientation"] && !value["orientation"].IsNull()) {
            orientation.yaw = value["orientation"]["yaw"].as<Param>();
            orientation.pitch = value["orientation"]["pitch"].as<Param>();
        } else {  // TODO: keep the orientation structure
            orientation.yaw = 0.0;
            orientation.pitch = 0.0;
        }

        // Setup Speaker structure
        Speaker s(index, position, orientation);

#ifdef TESTMODE
        s.printAll();
#endif  // TESTMODE

        speakers.insert({index, s});

        actvSpkIdx.push_back(index);
    }
    _initDistanceMatrix();

    // initialize the specific configs
    // every speaker is actvSpkIdx when at initialize point
    for (size_t i = 0; i < actvSpkIdx.size(); ++i) {
        trajVector.push_back(actvSpkIdx[i]);
    }
    std::sort(trajVector.begin(), trajVector.end());

    // topoMatrix every speaker is connected
    for (size_t i = 0; i < actvSpkIdx.size(); ++i) {
        Indexes tmp_idx = actvSpkIdx;  // deepcopy
        topoMatrix[actvSpkIdx[i]] = tmp_idx;
    }

    return true;
}

size_t SpeakerManager::getNumAllSpeakers() { return speakers.size(); }

size_t SpeakerManager::getNumActiveSpeakers() { return actvSpkIdx.size(); }

Indexes SpeakerManager::getActiveSpeakerIndexes() { return actvSpkIdx; }

Index SpeakerManager::getRandomIndex() {
    return actvSpkIdx[_getRandomIndexes(actvSpkIdx.size(), 1)[0]];
}

Speaker SpeakerManager::getSpeakerByIndex(Index spkrIdx) {
    auto it = speakers.find(spkrIdx);

    if (it == speakers.end()) {
        throw std::out_of_range("Index not found in SpeakerManager");
    }
    return it->second;
}

Pair SpeakerManager::getIndexesByTrajectory(Param trajVal) {
    trajVal = trajVal < 0.0 ? 0.0 : trajVal;

    trajVal = trajVal - std::floor(trajVal);

    Index scaleUp = std::ceil(trajVal * trajVector.size());
    Index scaleDown = std::floor(trajVal * trajVector.size());

    // wrap the index back to 0
    scaleUp = scaleUp % trajVector.size();

    Index lower = trajVector[scaleDown];
    Index upper = trajVector[scaleUp];

    return std::make_pair(lower, upper);
}

Param SpeakerManager::getPanningRatio(Param trajVal) {
    trajVal = trajVal < 0.0 ? 0.0 : trajVal;

    trajVal = trajVal - std::floor(trajVal);

    Param scaled = trajVal * trajVector.size();

    return (scaled - std::floor(scaled));
}

Pair SpeakerManager::get_indexs_by_geometry(std::vector<Param> pos,
                                            std::vector<bool> mask,
                                            std::string coordinate) {
    std::vector<Param> distance;
    std::vector<Index> indexs;

    assert(mask.size() == 3 &&
           "ERROR: The mask vector for geometry selection muss be in size 3.");

    Param tmp_distan;
    for (const auto& maps : speakers) {
        Index key = maps.first;
        Speaker spkr = maps.second;

        if (coordinate == "cartesian") {
            tmp_distan = abs(spkr.getX() - pos[0]) * mask[0] +
                         abs(spkr.getY() - pos[1]) * mask[1] +
                         abs(spkr.getZ() - pos[2]) * mask[2];

        } else {  // spherical TODO: make them in the same weight
            tmp_distan = abs(spkr.getAzimuth() - pos[0]) * mask[0] +
                         abs(spkr.getElevation() - pos[1]) * mask[1] +
                         abs(spkr.getDistance() - pos[2]) * mask[2];
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

    Index smallest = getSpeakerByIndex(indexs[first]).getIndex();
    Index second_small = getSpeakerByIndex(indexs[second]).getIndex();

    return std::make_pair(smallest, second_small);
}

Index SpeakerManager::getIndexesByTrigger(Param trigger, Mode mode) {
    // just return the original one when trigger doesn't close to 1.0
    if (!isEqualTo1(trigger, TRIGGER_THRESHOLD)) return currIdx;
    Index selected;
    // load all connected speakers from the topology matrix
    Indexes candidates = topoMatrix[currIdx];
    int numCandidates = candidates.size();
    // if only one speaker connected to the current one, just return it
    if (numCandidates == 1) {
        selected = candidates[0];
    } else {
        selected = candidates[_getRandomIndexes(numCandidates, 1)[0]];
    }

    // int n_candidates = candidates.size();

    // selected = candidates[_get_random_indexs(n_candidates, 1)[0]];
    // switch(mode){
    //     case 0: // "Random"
    //         selected = candidates[_get_random_indexs(n_candidates, 1)[0]];
    //         break;
    //     case 1: // "Nearest"
    //         selected = _find_nearest(currIdx, candidates);
    //         break;
    //     default:
    //         throw std::invalid_argument( "Unknow trigger mode.");
    // }
    currIdx = selected;

    return currIdx;
}

Params SpeakerManager::getDistanceVector(Index spkrIdx) {
    return distanceMatrix[spkrIdx];
}

void SpeakerManager::setActiveSpeakers(std::string action, Indexes spkrIdxes) {
    if (action == "set") {
        _setActiveSpeakerIndexs(spkrIdxes);
    } else if (action == "add") {
        _addActiveSpeakerIndexs(spkrIdxes);
    } else if (action == "del") {
        _delActiveSpeakerIndexs(spkrIdxes);
    } else {
        logger->logWarning("SpeakerManager::setActiveSpeakers unknown action " +
                           action);
    }

#ifdef TESTMODE
    printActiveSpeakerIndexs();
#endif  // TESTMODE
}

void SpeakerManager::setTrajectoryVector(Indexes spkrIdxes) {
    Indexes tmpTrajVector;
    for (size_t i = 0; i < spkrIdxes.size(); ++i) {
        if (!isInVec<Index>(spkrIdxes[i], actvSpkIdx)) {
            logger->logError(formatString(
                "SpeakerManager: speaker %d is not activated!", spkrIdxes[i]));
            return;
        } else {
            tmpTrajVector.push_back(spkrIdxes[i]);
        }
    }
    trajVector = tmpTrajVector;
#ifdef TESTMODE
    printTrajectoryVector();
#endif  // TESTMODE
}

void SpeakerManager::setTopoMatrix(std::string action, Indexes spkrIdxes) {
    if (action == "set") {
        _setTopoMatrixIndexes(spkrIdxes);
    } else if (action == "add") {
        _addTopoMatrixIndexes(spkrIdxes);
    } else if (action == "del") {
        _delTopoMatrixIndexes(spkrIdxes);
    } else {
        logger->logWarning("SpeakerManager::setTopoMatrix unknown action " +
                           action);
    }
#ifdef TESTMODE
    printTopoMatrix();
#endif  // TESTMODE
}

void SpeakerManager::setCurrentSpeaker(Index newIdx) {
    if (!isInVec<Index>(newIdx, actvSpkIdx)) {
        logger->logError(formatString(
            "SpeakerManager: speaker %d is not activated!", newIdx));
        return;
    } else {
        currIdx = newIdx;
    }

#ifdef TESTMODE
    logger->logDebug(
        formatString("EnvelopeGenerator::initialize currIdx %d", currIdx));
#endif  // TESTMODE
}

bool SpeakerManager::_isActivated(Index idx) {
    if (!isInVec<Index>(idx, actvSpkIdx)) {
        logger->logError(formatString("Speaker %d is not activated!", idx));
        return false;
    }
    return true;
}

void SpeakerManager::_setTopoMatrixIndexes(Indexes spkrIdxes) {
    Index mainSpkr = spkrIdxes[0];
    if (!_isActivated(mainSpkr)) return;

    Indexes tmpTopoVector;
    for (size_t i = 1; i < spkrIdxes.size(); ++i) {
        if (!_isActivated(spkrIdxes[i])) return;
        tmpTopoVector.push_back(spkrIdxes[i]);
    }
    topoMatrix[mainSpkr] = tmpTopoVector;
}

void SpeakerManager::_addTopoMatrixIndexes(Indexes spkrIdxes) {
    Index mainSpkr = spkrIdxes[0];
    if (!_isActivated(mainSpkr)) return;

    for (size_t i = 1; i < spkrIdxes.size(); ++i) {
        if (!_isActivated(spkrIdxes[i])) continue;
        if (isInVec<Index>(spkrIdxes[i], topoMatrix[mainSpkr])) continue;
        topoMatrix[mainSpkr].push_back(spkrIdxes[i]);
    }
}

void SpeakerManager::_delTopoMatrixIndexes(Indexes spkrIdxes) {
    Index mainSpkr = spkrIdxes[0];
    Indexes& connected = topoMatrix[mainSpkr];

    if (!_isActivated(mainSpkr)) return;
    for (size_t i = 1; i < spkrIdxes.size(); ++i) {
        if (!_isActivated(spkrIdxes[i])) continue;
        if (!isInVec<Index>(spkrIdxes[i], connected)) continue;

        connected.erase(
            std::remove(connected.begin(), connected.end(), spkrIdxes[i]),
            connected.end());
    }
}

void SpeakerManager::printParameters() {
    printActiveSpeakerIndexs();
    printTrajectoryVector();
    printTopoMatrix();
}

void SpeakerManager::printActiveSpeakerIndexs() {
    logger->logInfo("Active Speakers: ");
    logger->logInfo("    " + formatVector<Index>(actvSpkIdx));
}

void SpeakerManager::printTopoMatrix() {
    logger->logInfo("Topological Matrix: ");
    for (auto it = topoMatrix.begin(); it != topoMatrix.end(); ++it) {
        logger->logInfo("    " + std::to_string(it->first) + " | " +
                        formatVector<Index>(it->second));
    }
}

void SpeakerManager::printTrajectoryVector() {
    logger->logInfo("Trajectory Vector: ");
    logger->logInfo("    " + formatVector<Index>(trajVector));
}

void SpeakerManager::_initDistanceMatrix() {
    int n_speakers = getNumActiveSpeakers();
    for (int i = 0; i < n_speakers; ++i) {
        distanceMatrix[actvSpkIdx[i]] = {};
        for (int j = 0; j < n_speakers; ++j) {
            Speaker s1 = getSpeakerByIndex(actvSpkIdx[i]);
            Speaker s2 = getSpeakerByIndex(actvSpkIdx[j]);
            distanceMatrix[actvSpkIdx[i]].push_back(_calculateDistance(s1, s2));
        }
    }
}

Index SpeakerManager::_findNearest(Index spkrIdx, Indexes candidates) {
    // TODO: seems incorrect
    Param minDistance = distanceMatrix[spkrIdx][0];
    Index nearest = candidates[0];
    Param tmpDistance;
    for (size_t i = 1; i < candidates.size(); ++i) {
        tmpDistance = distanceMatrix[spkrIdx][i];
        if (minDistance > tmpDistance) {
            nearest = candidates[i];
            minDistance = tmpDistance;
        }
    }
    return nearest;
}

Param SpeakerManager::_calculateDistance(Speaker s1, Speaker s2) {
    Param dx = s1.getX() - s2.getX();
    Param dy = s1.getY() - s2.getY();
    Param dz = s1.getZ() - s2.getZ();

    Param distance = std::sqrt(dx * dx + dy * dy + dz * dz);

    return distance;
}

Indexes SpeakerManager::_getRandomIndexes(int l, int n) {
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

Cartesian SpeakerManager::_spherical2cartesian(Spherical spherical) {
    // test
    Cartesian cartesian;
    cartesian.x = spherical.distance * cos(spherical.elevation / 180.0 * PI) *
                  cos(spherical.azimuth / 180.0 * PI);
    cartesian.y = spherical.distance * cos(spherical.elevation / 180.0 * PI) *
                  sin(spherical.azimuth / 180.0 * PI);
    cartesian.z = spherical.distance * sin(spherical.elevation / 180.0 * PI);

    return cartesian;
}

Spherical SpeakerManager::_cartesian2spherical(Cartesian cartesian) {
    Spherical spherical;
    spherical.distance =
        sqrt(cartesian.x * cartesian.x + cartesian.y * cartesian.y +
             cartesian.z * cartesian.z);
    spherical.azimuth = atan2(cartesian.y, cartesian.x) / PI * 180.0;
    spherical.elevation = asin(cartesian.z / spherical.distance) / PI * 180.0;

    return spherical;
}

void SpeakerManager::_setActiveSpeakerIndexs(Indexes spkrIdxes) {
    actvSpkIdx.clear();
    for (size_t i = 0; i < spkrIdxes.size(); ++i) {
        auto it = speakers.find(spkrIdxes[i]);
        if (it == speakers.end()) {
            logger->logError(
                formatString("SpeakerManager::_set_actvSpkIdx_indexs unknow "
                             "speaker index %d!",
                             spkrIdxes[i]));
            return;
        }
        // add to actvSpkIdx
        if (isInVec<Index>(spkrIdxes[i], actvSpkIdx)) {
            logger->logWarning(
                formatString("SpeakerManager: index %d already added, ignored",
                             spkrIdxes[i]));
        } else {
            actvSpkIdx.push_back(spkrIdxes[i]);
        }
    }
    // reset also distance matrix and topoMatrix to init state
    distanceMatrix.clear();
    _initDistanceMatrix();

    trajVector.clear();
    for (size_t i = 0; i < actvSpkIdx.size(); ++i) {
        trajVector.push_back(actvSpkIdx[i]);
    }
    std::sort(trajVector.begin(), trajVector.end());

    topoMatrix.clear();
    for (size_t i = 0; i < actvSpkIdx.size(); ++i) {
        Indexes tmp_idx = actvSpkIdx;
        topoMatrix[actvSpkIdx[i]] = tmp_idx;
    }
}

void SpeakerManager::_addActiveSpeakerIndexs(Indexes spkrIdxes) {
    for (size_t i = 0; i < spkrIdxes.size(); ++i) {
        // check if the input index is valid
        if (!isInKey<Index, Speaker>(spkrIdxes[i], speakers)) {
            logger->logError("SpeakerManager unknow speaker index!");
            return;
        }
        // add to actvSpkIdx
        if (isInVec<Index>(spkrIdxes[i], actvSpkIdx)) {
            logger->logWarning(
                formatString("SpeakerManager: index %d already added, ignored",
                             spkrIdxes[i]));
        } else {
            actvSpkIdx.push_back(spkrIdxes[i]);
        }
    }
}

void SpeakerManager::_delActiveSpeakerIndexs(Indexes spkrIdxes) {
    for (size_t i = 0; i < spkrIdxes.size(); ++i) {
        // check if the input index is valid
        if (!isInKey<Index, Speaker>(spkrIdxes[i], speakers)) {
            logger->logError("SpeakerManager unknow speaker index!");
            return;
        }
        // remove from actvSpkIdx
        if (!isInVec<Index>(spkrIdxes[i], actvSpkIdx)) {
            logger->logWarning(
                formatString("SpeakerManager index %d already removed, ignored",
                             spkrIdxes[i]));
        } else {
            actvSpkIdx.erase(
                std::remove(actvSpkIdx.begin(), actvSpkIdx.end(), spkrIdxes[i]),
                actvSpkIdx.end());
        }
        // remove from trajVector
        if (!isInVec<Index>(spkrIdxes[i], trajVector)) {
            logger->logWarning(
                formatString("SpeakerManager index %d already removed, ignored",
                             spkrIdxes[i]));
        } else {
            trajVector.erase(
                std::remove(trajVector.begin(), trajVector.end(), spkrIdxes[i]),
                trajVector.end());
        }
        // remove from topology matrix
        if (isInKey<Index, Indexes>(spkrIdxes[i], topoMatrix)) {
            topoMatrix.erase(spkrIdxes[i]);
        }
        for (auto it = topoMatrix.begin(); it != topoMatrix.end(); ++it) {
            if (isInVec<Index>(spkrIdxes[i], it->second)) {
                it->second.erase(std::remove(it->second.begin(),
                                             it->second.end(), spkrIdxes[i]),
                                 it->second.end());
            }
        }
    }
}