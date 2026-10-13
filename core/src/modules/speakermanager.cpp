/**
 * @file speakermanager.cpp
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief  Speaker & SpeakerManager Classes Implementation
 * @date 2024-02-18
 *
 * @copyright Copyright (c) 2023-2025
 *
 */
#include "speakermanager.h"
using namespace zerr;

Speaker::Speaker(Index index, Position position, Orientation orientation)
    : index(index), position(position), orientation(orientation)
{
#ifdef TESTMODE
    logger.setLogLevel(LogLevel::INFO);
#endif // TESTMODE
}

void Speaker::printAll()
{
    logger.logInfo("-----------------------");
    _print_index();
    _print_position();
    _print_orientation();
}

void Speaker::_print_index() { logger.logInfo(formatString("Speaker ID: %d", index)); }

void Speaker::_print_position()
{
    logger.logInfo("Cartesian Position: ");
    logger.logInfo(formatString("    x: %.2f", position.cartesian.x));
    logger.logInfo(formatString("    y: %.2f", position.cartesian.y));
    logger.logInfo(formatString("    z: %.2f", position.cartesian.z));
    logger.logInfo("Spherical Position: ");
    logger.logInfo(formatString("    azimuth:   : %.2f", position.spherical.azimuth));
    logger.logInfo(formatString("    elevation: : %.2f", position.spherical.elevation));
    logger.logInfo(formatString("    distance:  : %.2f", position.spherical.distance));
}

void Speaker::_print_orientation()
{
    logger.logInfo("Orientation: ");
    logger.logInfo(formatString("    yaw:   : %.2f", orientation.yaw));
    logger.logInfo(formatString("    pitch: : %.2f", orientation.pitch));
}

SpeakerManager::SpeakerManager(ConfigPath spkrArry) : speakerArrayPath(spkrArry)
{
#ifdef TESTMODE
    logger.setLogLevel(LogLevel::INFO);
#endif // TESTMODE
    logger.logInfo("SpeakerManager::SpeakerManager " + speakerArrayPath);
}

bool SpeakerManager::initialize()
{
    speakers.clear();
    actvSpkIdx.clear();
    trajVector.clear();
    topoMatrix.clear();
    distanceMatrix.clear();
    // load speaker configuration file
    try {
        speakerArrayNode = speakerArrayText.empty() ? YAML::LoadFile(speakerArrayPath)
                                                    : YAML::Load(speakerArrayText);
    }
    catch (...) {
        logger.logError("SpeakerManager::initialize: Load speaker configuration " +
                        speakerArrayPath + " failed");
        return false;
    }
    // analysis speaker configuration file
    // load standard configuration of each speakers
    YAML::Node speakerNodes = speakerArrayNode["standard"];

    // yaml-cpp throws on a key or value of the wrong type; a bad file must come back as
    // "failed", never as an exception on a host's C call stack.
    try {
        for (auto it = speakerNodes.begin(); it != speakerNodes.end(); ++it) {
            YAML::Node key   = it->first;
            YAML::Node value = it->second;

            int index = key.as<int>(); // assign the key to speaker index

            Cartesian cartesian; // parse the position values in cartesian coordinate
            Spherical spherical; // parse the position values in spherical coordinate

            bool is_zero_cartesian = true;
            bool is_zero_spherical = true;

            if (value["position"]["cartesian"] && !value["position"]["cartesian"].IsNull()) {
                cartesian.x       = value["position"]["cartesian"]["x"].as<Param>();
                cartesian.y       = value["position"]["cartesian"]["y"].as<Param>();
                cartesian.z       = value["position"]["cartesian"]["z"].as<Param>();
                is_zero_cartesian = false;
            }

            if (value["position"]["spherical"] && !value["position"]["spherical"].IsNull()) {
                spherical.azimuth   = value["position"]["spherical"]["azimuth"].as<Param>();
                spherical.elevation = value["position"]["spherical"]["elevation"].as<Param>();
                spherical.distance  = value["position"]["spherical"]["distance"].as<Param>();
                is_zero_spherical   = false;
            }

            // a speaker needs a position: neither form given puts it at the origin, where
            // the spherical conversion divides by zero
            if (is_zero_cartesian && is_zero_spherical) {
                logger.logError(formatString("SpeakerManager::initialize: speaker %d has no "
                                             "cartesian or spherical position",
                                             index));
                return false;
            }
            // TODO: check if the given two types of coordinate the same
            // if (is_zero_cartesian && is_zero_spherical)
            // {check_coordinate_consistency(cartesian, spherical)}

            cartesian = is_zero_cartesian ? _spherical2cartesian(spherical)
                                          : cartesian; // fill the cartesian if it's not assigned
            spherical = is_zero_spherical ? _cartesian2spherical(cartesian)
                                          : spherical; // fill the spherical if it's not assigned

            Position position;
            position.cartesian = cartesian;
            position.spherical = spherical;

            Orientation orientation;
            if (value["orientation"] && !value["orientation"].IsNull()) {
                orientation.yaw   = value["orientation"]["yaw"].as<Param>();
                orientation.pitch = value["orientation"]["pitch"].as<Param>();
            }
            else { // TODO: keep the orientation structure
                orientation.yaw   = 0.0;
                orientation.pitch = 0.0;
            }

            // Setup Speaker structure
            Speaker s(index, position, orientation);

#ifdef TESTMODE
            s.printAll();
#endif // TESTMODE

            speakers.insert({index, s});

            actvSpkIdx.push_back(index);
        }
    }
    catch (const std::exception& e) {
        logger.logError("SpeakerManager::initialize: cannot parse " + speakerArrayPath + ": " +
                        e.what());
        speakers.clear();
        actvSpkIdx.clear();
        return false;
    }

    if (speakers.empty()) {
        logger.logError("SpeakerManager::initialize: no speakers under 'standard:' in " +
                        speakerArrayPath);
        return false;
    }
    _initDistanceMatrix();

    // initialize the specific configs
    // every speaker is actvSpkIdx when at initialize point
    for (auto idx : actvSpkIdx) {
        trajVector.push_back(idx);
    }
    std::sort(trajVector.begin(), trajVector.end());

    // topoMatrix every speaker is connected
    for (auto idx : actvSpkIdx) {
        topoMatrix[idx] = actvSpkIdx; // copy
    }

    return true;
}

size_t SpeakerManager::getNumAllSpeakers() { return speakers.size(); }

size_t SpeakerManager::getNumActiveSpeakers() { return actvSpkIdx.size(); }

Indexes SpeakerManager::getActiveSpeakerIndexes() { return actvSpkIdx; }

Index SpeakerManager::getRandomIndex()
{
    if (actvSpkIdx.empty()) {
        return currIdx;
    }
    return actvSpkIdx[_getRandomIndexes(actvSpkIdx.size(), 1)[0]];
}

Speaker SpeakerManager::getSpeakerByIndex(Index spkrIdx)
{
    auto it = speakers.find(spkrIdx);

    if (it == speakers.end()) {
        throw std::out_of_range("Index not found in SpeakerManager");
    }
    return it->second;
}

SpeakerPair SpeakerManager::getIndexesByTrajectory(Param trajVal)
{
    // audio-thread guard: an empty trajectory would divide by zero below
    if (trajVector.empty()) {
        return std::make_pair(currIdx, currIdx);
    }

    trajVal = trajVal < 0.0 ? 0.0 : trajVal;

    trajVal = trajVal - std::floor(trajVal);

    Index scaleUp   = std::ceil(trajVal * trajVector.size());
    Index scaleDown = std::floor(trajVal * trajVector.size());

    // wrap the index back to 0
    scaleUp = scaleUp % trajVector.size();

    Index lower = trajVector[scaleDown];
    Index upper = trajVector[scaleUp];

    return std::make_pair(lower, upper);
}

Param SpeakerManager::getPanningRatio(Param trajVal)
{
    trajVal = trajVal < 0.0 ? 0.0 : trajVal;

    trajVal = trajVal - std::floor(trajVal);

    Param scaled = trajVal * trajVector.size();

    return (scaled - std::floor(scaled));
}

SpeakerPair SpeakerManager::get_indexs_by_geometry(std::vector<Param> pos, std::vector<bool> mask,
                                                   std::string coordinate)
{
    std::vector<Param> distance;
    std::vector<Index> indexs;

    assert(mask.size() == 3 && "ERROR: The mask vector for geometry selection muss be in size 3.");

    Param tmp_distan;
    for (auto& [key, spkr] : speakers) {
        if (coordinate == "cartesian") {
            tmp_distan = abs(spkr.getX() - pos[0]) * mask[0] + abs(spkr.getY() - pos[1]) * mask[1] +
                         abs(spkr.getZ() - pos[2]) * mask[2];
        }
        else { // spherical TODO: make them in the same weight
            tmp_distan = abs(spkr.getAzimuth() - pos[0]) * mask[0] +
                         abs(spkr.getElevation() - pos[1]) * mask[1] +
                         abs(spkr.getDistance() - pos[2]) * mask[2];
        }
        distance.push_back(tmp_distan);
        indexs.push_back(key);
    }

    size_t first  = 0;
    size_t second = 1;

    for (size_t i = 2; i < distance.size(); ++i) {
        if (distance[i] < distance[first]) {
            second = first;
            first  = i;
        }
        else if (distance[i] < distance[second]) {
            second = i;
        }
    }

    Index smallest     = getSpeakerByIndex(indexs[first]).getIndex();
    Index second_small = getSpeakerByIndex(indexs[second]).getIndex();

    return std::make_pair(smallest, second_small);
}

Index SpeakerManager::getIndexesByTrigger(Param trigger, TriggerMode mode)
{
    // just return the original one when trigger doesn't close to 1.0
    if (!isEqualTo1(trigger, TRIGGER_THRESHOLD))
        return currIdx;
    Index selected;
    // load all connected speakers from the topology matrix. No row, or an empty one
    // (every link deleted, or the current speaker deactivated): stay where we are.
    // Audio-thread guard, so it stays silent; the mutators warn when they create it.
    auto row = topoMatrix.find(currIdx);
    if (row == topoMatrix.end() || row->second.empty()) {
        return currIdx;
    }
    const Indexes& candidates = row->second;
    int numCandidates         = candidates.size();
    // if only one speaker connected to the current one, just return it
    if (numCandidates == 1) {
        selected = candidates[0];
    }
    else {
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

Params SpeakerManager::getDistanceVector(Index spkrIdx)
{
    // find, not operator[]: never insert into the matrix from the audio thread
    auto it = distanceMatrix.find(spkrIdx);
    return it == distanceMatrix.end() ? Params{} : it->second;
}

void SpeakerManager::setActiveSpeakers(std::string action, Indexes spkrIdxes)
{
    if (action == "set") {
        _setActiveSpeakerIndexs(spkrIdxes);
    }
    else if (action == "add") {
        _addActiveSpeakerIndexs(spkrIdxes);
    }
    else if (action == "del") {
        _delActiveSpeakerIndexs(spkrIdxes);
    }
    else {
        logger.logWarning("SpeakerManager::setActiveSpeakers unknown action " + action);
    }

#ifdef TESTMODE
    printActiveSpeakerIndexs();
#endif // TESTMODE
}

void SpeakerManager::setTrajectoryVector(Indexes spkrIdxes)
{
    Indexes tmpTrajVector;
    for (size_t i = 0; i < spkrIdxes.size(); ++i) {
        if (!isInVec<Index>(spkrIdxes[i], actvSpkIdx)) {
            logger.logError(
                formatString("SpeakerManager: speaker %d is not activated!", spkrIdxes[i]));
            return;
        }
        else {
            tmpTrajVector.push_back(spkrIdxes[i]);
        }
    }
    trajVector = tmpTrajVector;
#ifdef TESTMODE
    printTrajectoryVector();
#endif // TESTMODE
}

void SpeakerManager::setTopoMatrix(std::string action, Indexes spkrIdxes)
{
    if (action == "set") {
        _setTopoMatrixIndexes(spkrIdxes);
    }
    else if (action == "add") {
        _addTopoMatrixIndexes(spkrIdxes);
    }
    else if (action == "del") {
        _delTopoMatrixIndexes(spkrIdxes);
    }
    else {
        logger.logWarning("SpeakerManager::setTopoMatrix unknown action " + action);
    }
#ifdef TESTMODE
    printTopoMatrix();
#endif // TESTMODE
}

void SpeakerManager::setCurrentSpeaker(Index newIdx)
{
    if (!isInVec<Index>(newIdx, actvSpkIdx)) {
        logger.logError(formatString("SpeakerManager: speaker %d is not activated!", newIdx));
        return;
    }
    else {
        currIdx = newIdx;
    }

#ifdef TESTMODE
    logger.logDebug(formatString("EnvelopeGenerator::initialize currIdx %d", currIdx));
#endif // TESTMODE
}

bool SpeakerManager::_isActivated(Index idx)
{
    if (!isInVec<Index>(idx, actvSpkIdx)) {
        logger.logError(formatString("Speaker %d is not activated!", idx));
        return false;
    }
    return true;
}

void SpeakerManager::_setTopoMatrixIndexes(Indexes spkrIdxes)
{
    if (spkrIdxes.empty()) {
        logger.logError("SpeakerManager: topo set needs a speaker number");
        return;
    }
    Index mainSpkr = spkrIdxes[0];
    if (!_isActivated(mainSpkr))
        return;

    Indexes tmpTopoVector;
    for (size_t i = 1; i < spkrIdxes.size(); ++i) {
        if (!_isActivated(spkrIdxes[i]))
            return;
        tmpTopoVector.push_back(spkrIdxes[i]);
    }
    topoMatrix[mainSpkr] = tmpTopoVector;
    if (tmpTopoVector.empty()) {
        logger.logWarning(formatString(
            "SpeakerManager: speaker %d has no connections; triggers will keep it there",
            mainSpkr));
    }
}

void SpeakerManager::_addTopoMatrixIndexes(Indexes spkrIdxes)
{
    if (spkrIdxes.empty()) {
        logger.logError("SpeakerManager: topo add needs a speaker number");
        return;
    }
    Index mainSpkr = spkrIdxes[0];
    if (!_isActivated(mainSpkr))
        return;

    for (size_t i = 1; i < spkrIdxes.size(); ++i) {
        if (!_isActivated(spkrIdxes[i]))
            continue;
        if (isInVec<Index>(spkrIdxes[i], topoMatrix[mainSpkr]))
            continue;
        topoMatrix[mainSpkr].push_back(spkrIdxes[i]);
    }
}

void SpeakerManager::_delTopoMatrixIndexes(Indexes spkrIdxes)
{
    if (spkrIdxes.empty()) {
        logger.logError("SpeakerManager: topo del needs a speaker number");
        return;
    }
    Index mainSpkr = spkrIdxes[0];
    if (!_isActivated(mainSpkr))
        return;
    // find after the activation check, so an inactive speaker never gets a row inserted
    auto row = topoMatrix.find(mainSpkr);
    if (row == topoMatrix.end())
        return;
    Indexes& connected = row->second;

    for (size_t i = 1; i < spkrIdxes.size(); ++i) {
        if (!_isActivated(spkrIdxes[i]))
            continue;
        if (!isInVec<Index>(spkrIdxes[i], connected))
            continue;

        connected.erase(std::remove(connected.begin(), connected.end(), spkrIdxes[i]),
                        connected.end());
    }
    if (connected.empty()) {
        logger.logWarning(formatString(
            "SpeakerManager: speaker %d has no connections left; triggers will keep it there",
            mainSpkr));
    }
}

void SpeakerManager::printParameters()
{
    printActiveSpeakerIndexs();
    printTrajectoryVector();
    printTopoMatrix();
}

void SpeakerManager::printActiveSpeakerIndexs()
{
    logger.logInfo("Active Speakers: ");
    logger.logInfo("    " + formatVector<Index>(actvSpkIdx));
}

void SpeakerManager::printTopoMatrix()
{
    logger.logInfo("Topological Matrix: ");
    for (const auto& [idx, connections] : topoMatrix) {
        logger.logInfo("    " + std::to_string(idx) + " | " + formatVector<Index>(connections));
    }
}

void SpeakerManager::printTrajectoryVector()
{
    logger.logInfo("Trajectory Vector: ");
    logger.logInfo("    " + formatVector<Index>(trajVector));
}

void SpeakerManager::_initDistanceMatrix()
{
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

Index SpeakerManager::_findNearest(Index spkrIdx, Indexes candidates)
{
    // TODO: seems incorrect
    Param minDistance = distanceMatrix[spkrIdx][0];
    Index nearest     = candidates[0];
    Param tmpDistance;
    for (size_t i = 1; i < candidates.size(); ++i) {
        tmpDistance = distanceMatrix[spkrIdx][i];
        if (minDistance > tmpDistance) {
            nearest     = candidates[i];
            minDistance = tmpDistance;
        }
    }
    return nearest;
}

Param SpeakerManager::_calculateDistance(Speaker s1, Speaker s2)
{
    Param dx = s1.getX() - s2.getX();
    Param dy = s1.getY() - s2.getY();
    Param dz = s1.getZ() - s2.getZ();

    Param distance = std::sqrt(dx * dx + dy * dy + dz * dz);

    return distance;
}

Indexes SpeakerManager::_getRandomIndexes(int l, int n)
{
    int idx;
    std::vector<int> random_indexs;
    // nothing to choose from: callers guard this, but an empty result beats a
    // uniform_int_distribution(0, -1)
    if (l <= 0 || n <= 0) {
        return random_indexs;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, l - 1);

    for (int i = 0; i < n; ++i) {
        idx = dis(gen);
        random_indexs.push_back(idx);
    }

    return random_indexs;
}

Cartesian SpeakerManager::_spherical2cartesian(Spherical spherical)
{
    // test
    Cartesian cartesian;
    cartesian.x = spherical.distance * cos(spherical.elevation / 180.0 * pi) *
                  cos(spherical.azimuth / 180.0 * pi);
    cartesian.y = spherical.distance * cos(spherical.elevation / 180.0 * pi) *
                  sin(spherical.azimuth / 180.0 * pi);
    cartesian.z = spherical.distance * sin(spherical.elevation / 180.0 * pi);

    return cartesian;
}

Spherical SpeakerManager::_cartesian2spherical(Cartesian cartesian)
{
    Spherical spherical;
    spherical.distance =
        sqrt(cartesian.x * cartesian.x + cartesian.y * cartesian.y + cartesian.z * cartesian.z);
    spherical.azimuth   = atan2(cartesian.y, cartesian.x) / pi * 180.0;
    spherical.elevation = asin(cartesian.z / spherical.distance) / pi * 180.0;

    return spherical;
}

void SpeakerManager::_setActiveSpeakerIndexs(Indexes spkrIdxes)
{
    // validate the whole list first: a typo must not leave the object with no speakers
    if (spkrIdxes.empty()) {
        logger.logError("SpeakerManager: active set needs at least one speaker");
        return;
    }
    for (Index idx : spkrIdxes) {
        if (speakers.find(idx) == speakers.end()) {
            logger.logError(
                formatString("SpeakerManager: unknown speaker %d; active set ignored", idx));
            return;
        }
    }

    actvSpkIdx.clear();
    for (size_t i = 0; i < spkrIdxes.size(); ++i) {
        // add to actvSpkIdx
        if (isInVec<Index>(spkrIdxes[i], actvSpkIdx)) {
            logger.logWarning(
                formatString("SpeakerManager: index %d already added, ignored", spkrIdxes[i]));
        }
        else {
            actvSpkIdx.push_back(spkrIdxes[i]);
        }
    }
    // reset also distance matrix and topoMatrix to init state
    distanceMatrix.clear();
    _initDistanceMatrix();

    trajVector.clear();
    for (auto idx : actvSpkIdx) {
        trajVector.push_back(idx);
    }
    std::sort(trajVector.begin(), trajVector.end());

    topoMatrix.clear();
    for (auto idx : actvSpkIdx) {
        topoMatrix[idx] = actvSpkIdx; // copy
    }
}

void SpeakerManager::_addActiveSpeakerIndexs(Indexes spkrIdxes)
{
    for (size_t i = 0; i < spkrIdxes.size(); ++i) {
        // check if the input index is valid
        if (!isInKey<Index, Speaker>(spkrIdxes[i], speakers)) {
            logger.logError("SpeakerManager unknow speaker index!");
            return;
        }
        // add to actvSpkIdx
        if (isInVec<Index>(spkrIdxes[i], actvSpkIdx)) {
            logger.logWarning(
                formatString("SpeakerManager: index %d already added, ignored", spkrIdxes[i]));
        }
        else {
            actvSpkIdx.push_back(spkrIdxes[i]);
        }
    }
}

void SpeakerManager::_delActiveSpeakerIndexs(Indexes spkrIdxes)
{
    // refuse before touching anything if the request would leave nothing to play from
    for (Index idx : spkrIdxes) {
        if (!isInKey<Index, Speaker>(idx, speakers)) {
            logger.logError(
                formatString("SpeakerManager: unknown speaker %d; active del ignored", idx));
            return;
        }
    }
    size_t activeLeft = 0;
    for (Index idx : actvSpkIdx) {
        if (!isInVec<Index>(idx, spkrIdxes))
            ++activeLeft;
    }
    if (activeLeft == 0) {
        logger.logError("SpeakerManager: active del would leave no active speaker; ignored");
        return;
    }
    size_t trajLeft = 0;
    for (Index idx : trajVector) {
        if (!isInVec<Index>(idx, spkrIdxes))
            ++trajLeft;
    }
    if (!trajVector.empty() && trajLeft == 0) {
        logger.logError(
            "SpeakerManager: active del would empty the trajectory; send a new traj first");
        return;
    }

    for (size_t i = 0; i < spkrIdxes.size(); ++i) {
        // remove from actvSpkIdx
        if (!isInVec<Index>(spkrIdxes[i], actvSpkIdx)) {
            logger.logWarning(
                formatString("SpeakerManager index %d already removed, ignored", spkrIdxes[i]));
        }
        else {
            actvSpkIdx.erase(std::remove(actvSpkIdx.begin(), actvSpkIdx.end(), spkrIdxes[i]),
                             actvSpkIdx.end());
        }
        // remove from trajVector
        if (!isInVec<Index>(spkrIdxes[i], trajVector)) {
            logger.logWarning(
                formatString("SpeakerManager index %d already removed, ignored", spkrIdxes[i]));
        }
        else {
            trajVector.erase(std::remove(trajVector.begin(), trajVector.end(), spkrIdxes[i]),
                             trajVector.end());
        }
        // remove from topology matrix
        if (isInKey<Index, Indexes>(spkrIdxes[i], topoMatrix)) {
            topoMatrix.erase(spkrIdxes[i]);
        }
        for (auto& [idx, connections] : topoMatrix) {
            if (isInVec<Index>(spkrIdxes[i], connections)) {
                connections.erase(std::remove(connections.begin(), connections.end(), spkrIdxes[i]),
                                  connections.end());
            }
        }
    }

    // the current speaker may have been deleted: move to the first one still active
    if (!isInVec<Index>(currIdx, actvSpkIdx) && !actvSpkIdx.empty()) {
        logger.logWarning(formatString("SpeakerManager: current speaker %d deactivated; now on %d",
                                       currIdx, actvSpkIdx.front()));
        currIdx = actvSpkIdx.front();
    }
}
