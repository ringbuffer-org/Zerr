/**
 * @file speakermanager.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief  Speaker & SpeakerManager Classes Header
 * @date 2024-02-18
 *
 * @copyright Copyright (c) 2023-2024
 *
 * This header file defines the Speaker and SpeakerManager classes used to
 * manage speaker configurations and behaviors in the zerr namespace. It
 * includes functionalities for initializing speaker instances with specific
 * configurations, managing speaker arrays, and performing various operations
 * related to speaker selection and properties.
 */

#ifndef SPEAKERMANAGER_H
#define SPEAKERMANAGER_H

#include <cmath>
#include <iostream>
#include <random>

#include "configs.h"
#include "logger.h"
#include "types.h"
#include "utils.h"
#include "yaml-cpp/yaml.h"

namespace zerr {

/**
 * @class Speaker
 * @brief Models the basic properties of a physical speaker.
 *
 * This class provides methods to access and manipulate the properties
 * of a speaker, including its index, position (in both Cartesian and spherical
 * coordinates), and orientation (yaw and pitch).
 */
class Speaker {
  public:
    /**
     * @brief Construct a new Speaker object with specified configurations.
     * @param index Unique identification number of the speaker.
     * @param position Position of the speaker in Cartesian and spherical
     * coordinates.
     * @param orientation Orientation of the speaker in terms of yaw and pitch.
     */
    Speaker(Index index, Position position, Orientation orientation);

    /**
     * @brief Get the identification index of this speaker instance.
     * @return Index The unique identification number of the speaker.
     */
    Index getIndex() { return index; };

    /**
     * @brief Get the x coordinate in Cartesian system of this speaker instance.
     * @return Param The x coordinate value.
     */
    Param getX() { return position.cartesian.x; };

    /**
     * @brief Get the y coordinate in Cartesian system of this speaker instance.
     * @return Param The y coordinate value.
     */
    Param getY() { return position.cartesian.y; };

    /**
     * @brief Get the z coordinate in Cartesian system of this speaker instance.
     * @return Param The z coordinate value.
     */
    Param getZ() { return position.cartesian.z; };

    /**
     * @brief Get the azimuth in spherical system of this speaker instance.
     * @return Param The azimuth value.
     */
    Param getAzimuth() { return position.spherical.azimuth; };

    /**
     * @brief Get the elevation in spherical system of this speaker instance.
     * @return Param The elevation value.
     */
    Param getElevation() { return position.spherical.elevation; };

    /**
     * @brief Get the distance in spherical system of this speaker instance.
     * @return Param The distance value.
     */
    Param getDistance() { return position.spherical.distance; };

    /**
     * @brief Get the yaw orientation of this speaker instance.
     * @return Param The yaw value.
     */
    Param getYaw() { return orientation.yaw; };

    /**
     * @brief Get the pitch orientation of this speaker instance.
     * @return Param The pitch value.
     */
    Param getPitch() { return orientation.pitch; };

    /**
     * @brief Print all configurations of the speaker, including index,
     * position, and orientation.
     */
    void printAll();

  private:
    Logger *logger;     ///< Logger object for logging.
    Index index;        ///< Unique identification number of the speaker.
    Position position;  ///< Position of the speaker in both Cartesian and
                        ///< spherical coordinates.
    Orientation
        orientation;  ///< Orientation of the speaker in terms of yaw and pitch.

    void _print_index();  ///< Internal method to print the speaker's index.
    void
    _print_position();  ///< Internal method to print the speaker's position.
    void _print_orientation();  ///< Internal method to print the speaker's
                                ///< orientation.
};

/**
 * @class SpeakerManager
 * @brief Manages speaker array configurations and output speaker selection
 * behaviors.
 *
 * This class is responsible for loading speaker array configurations from a
 * file, initializing speakers based on the configurations, and managing the
 * selection of output speakers based on various criteria.
 */
class SpeakerManager {
  public:
    /**
     * @brief Construct a new SpeakerManager object and set the configuration
     * file path.
     * @param spkrArry Path to the speaker array configuration file.
     */
    SpeakerManager(ConfigPath spkrArry);

    /**
     * @brief Initialize the speaker array using the configuration file.
     * @return bool True if initialization is successful, false otherwise.
     */
    bool initialize();

    /**
     * @brief Get the total number of speakers managed by this instance.
     * @return size_t The number of speakers.
     */
    size_t getNumAllSpeakers();

    /**
     * @brief Get the number of active speakers.
     * @return size_t The number of active speakers.
     */
    size_t getNumActiveSpeakers();

    /**
     * @brief Get the indexes of active speakers.
     * @return Indexes A vector of indexes representing the active speakers.
     */
    Indexes getActiveSpeakerIndexes();

    /**
     * @brief Select a random index from the active speakers.
     * @return Index A randomly selected active speaker index.
     */
    Index getRandomIndex();

    /**
     * @brief Get the speaker instance by its index.
     * @param spkrIdx The index of the speaker to retrieve.
     * @return Speaker The speaker instance corresponding to the given index.
     */
    Speaker getSpeakerByIndex(Index spkrIdx);

    /**
     * @brief Get a pair of speaker indexes based on the trajectory input value.
     * @param trajVal The trajectory input value used to determine speaker
     * selection.
     * @return Pair A pair of speaker indexes selected based on the trajectory
     * value.
     */
    Pair getIndexesByTrajectory(Param trajVal);

    /**
     * @brief Calculate the panning ratio based on the trajectory input value.
     * This method is intended for calculating how audio should be panned
     * between two speakers based on a specific trajectory value.
     * @param trajVal The trajectory input value.
     * @return Param The calculated panning ratio.
     */
    Param getPanningRatio(Param trajVal);

    /**
     * @brief Get the indexes of speakers that are geometrically closest to the
     * given position. This method is marked as obsolete and is planned for
     * removal in future versions.
     * @param pos The target position in either Cartesian or spherical
     * coordinates.
     * @param mask A boolean mask to include or exclude certain speakers.
     * @param coordinate The coordinate system of the pos parameter ("Cartesian"
     * or "spherical").
     * @return Pair A pair of speaker indexes closest to the given position.
     */
    Pair get_indexs_by_geometry(std::vector<Param> pos, std::vector<bool> mask,
                                std::string coordinate);

    /**
     * @brief Select a speaker index based on a trigger signal and the current
     * mode.
     * @param trigger The trigger signal value.
     * @param mode The operation mode that determines how the trigger affects
     * speaker selection.
     * @return Index The selected speaker index based on the trigger.
     */
    Index getIndexesByTrigger(Param trigger, Mode mode);

    /**
     * @brief Get a vector of distances from a specific speaker index to all
     * other speakers.
     * @param spkrIdx The index of the speaker from which distances are
     * measured.
     * @return Params A vector containing the distances to all other speakers.
     */
    Params getDistanceVector(Index spkrIdx);

    /**
     * @brief Activate or deactivate speakers based on the given action and
     * speaker indexes.
     * @param action The action to be performed ("set", "add", "del").
     * @param spkrIdxes The speaker indexes to which the action will be applied.
     */
    void setActiveSpeakers(std::string action, Indexes spkrIdxes);

    /**
     * @brief Set the trajectory vector, which defines a sequence of speaker
     * indexes for audio playback.
     * @param spkrIdxes A vector of speaker indexes defining the trajectory.
     */
    void setTrajectoryVector(Indexes spkrIdxes);

    /**
     * @brief Set or update the topology matrix based on specified actions and
     * speaker indexes. The topology matrix is used to manage spatial
     * relationships between speakers.
     * @param action The action to be performed ("set", "add", "del").
     * @param spkrIdxes The speaker indexes to which the action will be applied.
     */
    void setTopoMatrix(std::string action, Indexes spkrIdxes);

    /**
     * @brief Set the current active speaker to a new index.
     * @param newIdx The new speaker index to set as current.
     */
    void setCurrentSpeaker(Index newIdx);

    /**
     * @brief Print parameters related to the speaker manager's configuration
     * and state.
     */
    void printParameters();

    /**
     * @brief Print the indexes of currently active speakers.
     */
    void printActiveSpeakerIndexs();

    /**
     * @brief Print the topology matrix, showing spatial relationships between
     * speakers.
     */
    void printTopoMatrix();

    /**
     * @brief Print the trajectory vector, showing the sequence of speaker
     * indexes for audio playback.
     */
    void printTrajectoryVector();

  private:
    Logger *logger;  ///< Pointer to a Logger object for logging messages in
                     ///< different environment.
    ConfigPath
        speakerArrayPath;  ///< Path to the speaker array configuration file.
    YAML::Node speakerArrayNode;  ///< YAML Node structure containing the loaded
                                  ///< speaker array configuration.

    std::map<Index, Speaker>
        speakers;  ///< Map of speaker indexes to Speaker objects.
    std::map<Index, Params> distanceMatrix;  ///< Map of each speaker index and
                                             ///< the distance to other speakers

    Index currIdx;       ///< Current active speaker index.
    Indexes actvSpkIdx;  ///< Vector of indexes for currently active speakers.
    Indexes trajVector;  ///< Vector of speaker indexes defining the speaker
                         ///< trajectory.
    TopoMatrix topoMatrix;  ///< Matrix defining the topology or spatial
                            ///< relationship between speakers.

    /**
     * @brief Initializes the distance matrix used for spatial calculations.
     */
    void _initDistanceMatrix();

    /**
     * @brief Generates a vector of random speaker indexes.
     * @param l The lower bound for random index generation.
     * @param n The number of random indexes to generate.
     * @return Indexes A vector of randomly generated speaker indexes.
     */
    Indexes _getRandomIndexes(int l, int n);

    /**
     * @brief Calculates the distance between two speakers.
     * @param s1 The first speaker.
     * @param s2 The second speaker.
     * @return Param The calculated distance between the two speakers.
     */
    Param _calculateDistance(Speaker s1, Speaker s2);

    /**
     * @brief Converts spherical coordinates to Cartesian coordinates.
     * @param spherical The spherical coordinates to convert.
     * @return Cartesian The resulting Cartesian coordinates.
     */
    Cartesian _spherical2cartesian(Spherical spherical);

    /**
     * @brief Converts Cartesian coordinates to spherical coordinates.
     * @param cartesian The Cartesian coordinates to convert.
     * @return Spherical The resulting spherical coordinates.
     */
    Spherical _cartesian2spherical(Cartesian cartesian);

    /**
     * @brief Finds the nearest speaker index from a given index within a list
     * of candidate indexes.
     * @param spkrIdx The current speaker index.
     * @param candidates The list of candidate speaker indexes.
     * @return Index The index of the nearest speaker.
     */
    Index _findNearest(Index spkrIdx, Indexes candidates);

    /**
     * @brief Sets the indexes for active speakers.
     * @param spkrIdxes The indexes to set as active.
     */
    void _setActiveSpeakerIndexs(Indexes spkrIdxes);

    /**
     * @brief Adds indexes to the list of active speakers.
     * @param spkrIdxes The indexes to add to the active list.
     */
    void _addActiveSpeakerIndexs(Indexes spkrIdxes);

    /**
     * @brief Removes indexes from the list of active speakers.
     * @param spkrIdxes The indexes to remove from the active list.
     */
    void _delActiveSpeakerIndexs(Indexes spkrIdxes);

    /**
     * @brief Sets indexes in the topology matrix.
     * @param spkrIdxes The indexes to set in the topology matrix.
     */
    void _setTopoMatrixIndexes(Indexes spkrIdxes);

    /**
     * @brief Adds indexes to the topology matrix.
     * @param spkrIdxes The indexes to add to the topology matrix.
     */
    void _addTopoMatrixIndexes(Indexes spkrIdxes);

    /**
     * @brief Removes indexes from the topology matrix.
     * @param spkrIdxes The indexes to remove from the topology matrix.
     */
    void _delTopoMatrixIndexes(Indexes spkrIdxes);

    /**
     * @brief Checks if a speaker index is currently activated.
     * @param spkrIdx The index to check.
     * @return bool True if the speaker is activated, false otherwise.
     */
    bool _isActivated(Index spkrIdx);
};

#ifdef PUREDATA  // explicit instantiation required for PD
template bool isInKey<int, Speaker>(int element, std::map<Index, Speaker> map);
template std::string formatVector<Index>(std::vector<Index> vector);
#endif  // PUREDATA

}  // namespace zerr

#endif  // SPEAKERMANAGER_H