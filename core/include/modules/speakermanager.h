/**
 * @file speakermanager.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief  Speaker & SpeakerManager Classes Header - Contains class definitions for managing speaker configurations and behaviors
 * @date 2024-02-18
 *
 * @copyright Copyright (c) 2023-2025
 *
 * This header file defines the Speaker and SpeakerManager classes used to
 * manage speaker configurations and behaviors in the zerr namespace. It
 * includes functionalities for initializing speaker instances with specific
 * configurations, managing speaker arrays, and performing various operations
 * related to speaker selection and properties. The Speaker class models individual
 * speaker properties like position and orientation, while SpeakerManager handles
 * the overall configuration and control of multiple speakers, including speaker
 * selection, activation, and spatial relationships.
 */

#ifndef SPEAKERMANAGER_H
#define SPEAKERMANAGER_H

#include <cmath>
#include <iostream>
#include <random>
#include <functional>

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
 * coordinates), and orientation (yaw and pitch). It encapsulates all the physical
 * attributes needed to represent a speaker in 3D space and provides methods for
 * accessing and displaying these properties.
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
     * @return Param The azimuth value in degrees, where 0° is front center and positive values rotate clockwise.
     */
    Param getAzimuth() { return position.spherical.azimuth; };

    /**
     * @brief Get the elevation in spherical system of this speaker instance.
     * @return Param The elevation value in degrees, where 0° is horizontal and positive values point upward.
     */
    Param getElevation() { return position.spherical.elevation; };

    /**
     * @brief Get the distance in spherical system of this speaker instance.
     * @return Param The distance value in meters from the origin to the speaker.
     */
    Param getDistance() { return position.spherical.distance; };

    /**
     * @brief Get the yaw orientation of this speaker instance.
     * @return Param The yaw value in degrees, representing horizontal rotation of the speaker.
     */
    Param getYaw() { return orientation.yaw; };

    /**
     * @brief Get the pitch orientation of this speaker instance.
     * @return Param The pitch value in degrees, representing vertical tilt of the speaker.
     */
    Param getPitch() { return orientation.pitch; };

    /**
     * @brief Print all configurations of the speaker, including index,
     * position, and orientation.
     */
    void printAll();

 private:
    Logger* logger; ///< Logger object for logging messages and errors.
    Index index; ///< Unique identification number of the speaker.
    Position position; ///< Position of the speaker in both Cartesian and
                       ///< spherical coordinates.
    Orientation orientation; ///< Orientation of the speaker in terms of yaw and pitch.

    void _print_index(); ///< Internal method to print the speaker's index to console.
    void _print_position(); ///< Internal method to print the speaker's position coordinates.
    void _print_orientation(); ///< Internal method to print the speaker's
                               ///< orientation angles.
};

/**
 * @class SpeakerManager
 * @brief Manages speaker array configurations and output speaker selection
 * behaviors.
 *
 * This class is responsible for loading speaker array configurations from a
 * file, initializing speakers based on the configurations, and managing the
 * selection of output speakers based on various criteria. It provides functionality
 * for speaker activation/deactivation, trajectory management, topology control,
 * and various speaker selection algorithms based on spatial relationships and
 * user-defined patterns.
 */
class SpeakerManager {
 public:
    /**
     * @brief Construct a new SpeakerManager object and set the configuration
     * file path.
     * @param spkrArry Path to the speaker array configuration file in YAML format.
     */
    SpeakerManager(ConfigPath spkrArry);

    /**
     * @brief Initialize the speaker array using the configuration file.
     * @return bool True if initialization is successful, false otherwise.
     */
    bool initialize();

    /**
     * @brief Get the total number of speakers managed by this instance.
     * @return size_t The total number of speakers in the configuration.
     */
    size_t getNumAllSpeakers();

    /**
     * @brief Get the number of active speakers.
     * @return size_t The number of currently active speakers.
     */
    size_t getNumActiveSpeakers();

    /**
     * @brief Get the indexes of active speakers.
     * @return Indexes A vector of indexes representing the currently active speakers.
     */
    Indexes getActiveSpeakerIndexes();

    /**
     * @brief Select a random index from the active speakers.
     * @return Index A randomly selected index from the set of active speakers.
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
     * @param trajVal The trajectory input value between 0.0 and 1.0 used to determine speaker
     * selection along the defined trajectory.
     * @return Pair A pair of adjacent speaker indexes selected based on the trajectory
     * value.
     */
    Pair getIndexesByTrajectory(Param trajVal);

    /**
     * @brief Calculate the panning ratio based on the trajectory input value.
     * This method is intended for calculating how audio should be panned
     * between two speakers based on a specific trajectory value.
     * @param trajVal The trajectory input value between 0.0 and 1.0.
     * @return Param The calculated panning ratio between 0.0 and 1.0.
     */
    Param getPanningRatio(Param trajVal);

    /**
     * @brief Get the indexes of speakers that are geometrically closest to the
     * given position. This method is marked as obsolete and is planned for
     * removal in future versions.
     * @param pos The target position coordinates.
     * @param mask A boolean vector to include (true) or exclude (false) specific speakers.
     * @param coordinate The coordinate system to use ("Cartesian" or "spherical").
     * @return Pair A pair of speaker indexes closest to the given position.
     */
    Pair get_indexs_by_geometry(std::vector<Param> pos, std::vector<bool> mask,
        std::string coordinate);

    /**
     * @brief Select a speaker index based on a trigger signal and the current
     * mode.
     * @param trigger The trigger signal value that initiates speaker selection.
     * @param mode The operation mode that determines the selection algorithm.
     * @return Index The selected speaker index based on the trigger and mode.
     */
    Index getIndexesByTrigger(Param trigger, Mode mode);

    /**
     * @brief Get a vector of distances from a specific speaker index to all
     * other speakers.
     * @param spkrIdx The reference speaker index.
     * @return Params A vector containing the Euclidean distances to all other speakers.
     */
    Params getDistanceVector(Index spkrIdx);

    /**
     * @brief Activate or deactivate speakers based on the given action and
     * speaker indexes.
     * @param action The action to perform: "set" (replace current), "add" (activate), or "del" (deactivate).
     * @param spkrIdxes The speaker indexes to modify.
     */
    void setActiveSpeakers(std::string action, Indexes spkrIdxes);

    /**
     * @brief Set the trajectory vector, which defines a sequence of speaker
     * indexes for audio playback.
     * @param spkrIdxes An ordered vector of speaker indexes defining the playback path.
     */
    void setTrajectoryVector(Indexes spkrIdxes);

    /**
     * @brief Set or update the topology matrix based on specified actions and
     * speaker indexes. The topology matrix is used to manage spatial
     * relationships between speakers.
     * @param action The action to perform: "set" (replace), "add" (create connection), or "del" (remove connection).
     * @param spkrIdxes The speaker indexes to modify in the topology.
     */
    void setTopoMatrix(std::string action, Indexes spkrIdxes);

    /**
     * @brief Set the current active speaker to a new index.
     * @param newIdx The speaker index to set as the current active speaker.
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

    void setPrinter(Logger::PrintStrategy newPrinter)
    {
        // The logger of SpeakerManger
        logger->setPrinter(newPrinter);
    }

    Logger* logger; ///< Pointer to a Logger object for logging messages and errors
                    ///< in different environments.

 private:
    ConfigPath speakerArrayPath; ///< Path to the YAML speaker array configuration file.
    YAML::Node speakerArrayNode; ///< YAML Node containing the parsed speaker
                                 ///< array configuration data.

    std::map<Index, Speaker> speakers; ///< Map associating speaker indexes with their Speaker objects.
    std::map<Index, Params> distanceMatrix; ///< Matrix storing pre-calculated
                                            ///< distances between all speaker pairs.

    Index currIdx; ///< Index of the currently selected speaker.
    Indexes actvSpkIdx; ///< Vector storing indexes of all currently active speakers.
    Indexes trajVector; ///< Ordered vector of speaker indexes defining the spatial
                        ///< trajectory for playback.
    TopoMatrix topoMatrix; ///< Matrix defining the connectivity and spatial
                           ///< relationships between speakers.

    /**
     * @brief Initializes the distance matrix used for spatial calculations.
     * Computes and stores distances between all speaker pairs.
     */
    void _initDistanceMatrix();

    /**
     * @brief Generates a vector of random speaker indexes.
     * @param l The lower bound for random index generation.
     * @param n The number of random indexes to generate.
     * @return Indexes A vector of n unique random speaker indexes >= l.
     */
    Indexes _getRandomIndexes(int l, int n);

    /**
     * @brief Calculates the Euclidean distance between two speakers.
     * @param s1 The first speaker object.
     * @param s2 The second speaker object.
     * @return Param The calculated distance in meters between the speakers.
     */
    Param _calculateDistance(Speaker s1, Speaker s2);

    /**
     * @brief Converts spherical coordinates to Cartesian coordinates.
     * @param spherical The spherical coordinates (azimuth, elevation, distance).
     * @return Cartesian The equivalent Cartesian coordinates (x, y, z).
     */
    Cartesian _spherical2cartesian(Spherical spherical);

    /**
     * @brief Converts Cartesian coordinates to spherical coordinates.
     * @param cartesian The Cartesian coordinates (x, y, z).
     * @return Spherical The equivalent spherical coordinates (azimuth, elevation, distance).
     */
    Spherical _cartesian2spherical(Cartesian cartesian);

    /**
     * @brief Finds the nearest speaker index from a given index within a list
     * of candidate indexes.
     * @param spkrIdx The reference speaker index.
     * @param candidates Vector of candidate speaker indexes to search.
     * @return Index The index of the nearest speaker from the candidates.
     */
    Index _findNearest(Index spkrIdx, Indexes candidates);

    /**
     * @brief Sets the indexes for active speakers, replacing any existing ones.
     * @param spkrIdxes The new set of speaker indexes to activate.
     */
    void _setActiveSpeakerIndexs(Indexes spkrIdxes);

    /**
     * @brief Adds new indexes to the existing list of active speakers.
     * @param spkrIdxes The speaker indexes to add to the active list.
     */
    void _addActiveSpeakerIndexs(Indexes spkrIdxes);

    /**
     * @brief Removes specified indexes from the list of active speakers.
     * @param spkrIdxes The speaker indexes to deactivate.
     */
    void _delActiveSpeakerIndexs(Indexes spkrIdxes);

    /**
     * @brief Sets new connections in the topology matrix, replacing existing ones.
     * @param spkrIdxes The speaker indexes defining the new topology connections.
     */
    void _setTopoMatrixIndexes(Indexes spkrIdxes);

    /**
     * @brief Adds new connections to the existing topology matrix.
     * @param spkrIdxes The speaker indexes to connect in the topology.
     */
    void _addTopoMatrixIndexes(Indexes spkrIdxes);

    /**
     * @brief Removes specified connections from the topology matrix.
     * @param spkrIdxes The speaker indexes whose connections should be removed.
     */
    void _delTopoMatrixIndexes(Indexes spkrIdxes);

    /**
     * @brief Checks if a speaker index is currently activated.
     * @param spkrIdx The speaker index to check.
     * @return bool True if the speaker is in the active list, false otherwise.
     */
    bool _isActivated(Index spkrIdx);
};

#ifdef PUREDATA // explicit instantiation required for PD
template bool isInKey<int, Speaker>(int element, std::map<Index, Speaker> map);
template std::string formatVector<Index>(std::vector<Index> vector);
#endif // PUREDATA

} // namespace zerr

#endif // SPEAKERMANAGER_H
