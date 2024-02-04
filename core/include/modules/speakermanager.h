/**
 * @file speakermanager.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief  Speaker & SpeakerManager Class Implementation
 * @date 2023-08-05
 * 
 * @copyright Copyright (c) 2023-2024
 */
#ifndef SPEAKERMANAGER_H
#define SPEAKERMANAGER_H
#include <iostream>
#include <random>
#include <cmath>

#include "yaml-cpp/yaml.h"

#include "utils.h"
#include "types.h"
#include "configs.h"
#include "logger.h"

namespace zerr {
/**
* Speaker Class to model the basic properties of a physical speaker
*/
class Speaker {
 public:
    /**
    * @brief assgining index and geometry configuration when creating speaker instance
    * @param index the uniqe identification index of Speaker
    * @param position cartesian and spherical coordinates
    * @param orientation yaw & pitch orientation
    */
    Speaker(t_index index, t_position position, t_orientation orientation);
    /**
    * @brief return the identification index of this speaker instance
    * @return t_index index number
    */
    t_index get_index(){return index;}; 
    /**
    * @brief return the x in Cartesian coordinate of this speaker instance
    * @return x coordinate value
    */
    t_value get_x(){return position.cartesian.x;};
    /**
    * @brief return the y in Cartesian coordinate of this speaker instance
    * @return y coordinate value
    */
    t_value get_y(){return position.cartesian.y;};
    /**
    * @brief return the z in Cartesian coordinate of this speaker instance
    */
    t_value get_z(){return position.cartesian.z;};
    /**
    * @brief return the azimuth in spherical coordinate of this speaker instance
    */
    t_value get_azimuth()  {return position.spherical.azimuth;};
    /**
    * @brief return the elevation in spherical coordinate of this speaker instance
    */
    t_value get_elevation(){return position.spherical.elevation;};
    /**
    * @brief return the distance in spherical coordinate of this speaker instance
    */
    t_value get_distance() {return position.spherical.distance;};
    /**
    * @brief return the yaw in orientation of this speaker instance
    */
    t_value get_yaw()  {return orientation.yaw;};
    /**
    * @brief return the pitch in orientation of this speaker instance
    */
    t_value get_pitch(){return orientation.pitch;};
    /**
    * @brief pring all configuration: index, position, orientation
    */
    void print_all();

 private:
    Logger        *logger;      /**< logger  */
    t_index        index;       /**< TODO */
    t_position     position;    /**< TODO */
    t_orientation  orientation; /**< TODO */
    /**
    * @brief 
    */
    void _print_index();
    /**
    * @brief
    */
    void _print_position();
    /**
    * @brief
    */
    void _print_orientation();
};


/**
* @brief SpeakerManager class is for loading speaker array configuration file 
* and accessing & choosing speakers
*/
class SpeakerManager {
 public:
    SpeakerManager(std::string config_path);
    /**
    * @brief setup speaker array via the YAML config file and specified subgroup mode
    */
    bool initialize();
    /**
    * @brief TODO
    */
    void reset(std::string config_path);
    /**
    * @brief get the number of speakers in this speaker array setup
    */
    size_t getNumAllSpeakers(); 
    /**
    * @brief get the number of speakers in this speaker array setup
    */
    size_t getNumActiveSpeakers(); 
    /**
    * @brief TODO
    */
    t_indexs getActiveSpeakerIndexs();
    /**
    * @brief TODO
    */
    t_index get_random_index();
    /**
    * @brief return the speaker class acoording to speaker index
    */
    Speaker get_speaker_by_index(t_index spkr_idx);
    /**
    * @brief get the pair of speaker indexs in sorted list acoording to the trajectory input
    */
    t_pair get_indexs_by_trajectory(t_value trajectory_val);
    /**
    * @brief TODO
    */
    t_value get_panning_ratio(t_value trajectory_val);
    /**
    * @brief get the index of speaker that is geometrially close to the input. 
    *        This method is obsolete and will be removed in the future.
    */
    t_pair get_indexs_by_geometry(std::vector<t_value> pos, std::vector<bool> mask, std::string coordinate);
    /**
    * @brief jump between different speakers using trigger signal
    * the 
    */
    t_index get_indexs_by_trigger(t_value trigger, t_index curr_spkr, int mode);
    /**
    * @brief TODO
    */
    std::vector<float> get_distance_vector(int spkr_idx);
    /**
    * @brief TODO
    */
    void setTrajectoryVector(t_indexs idxs);
    /**
    * @brief TODO
    */
    void setTopoMatrix(TopoMatrix matrix);
    /**
    * @brief TODO
    */
    void setActiveSpeakers(std::string action, t_indexs idxs);
    /**
    * @brief TODO
    */
    void printActiveSpeakerIndexs();
    /**
    * @brief TODO
    */
    void printTopoMatrix();
    /**
    * @brief TODO
    */
    void printTrajectoryVector();
 private:
    Logger *logger;             /**< logger object for logging message in different environment */

    std::string config_path;    /**< file path of speaker array configuration */
    YAML::Node speaker_config;  /**< loaded YAML Node structure of speaker array configuration */

    std::map<t_index, Speaker> speakers;         /**< TODO  */

    std::map<t_index, t_values> distance_matrix; /**< TODO */

    // specific speaker configurations
    t_indexs                     actvSpkIdx;    /**< TODO */
    t_indexs                     trajVector;    /**< TODO */
    TopoMatrix                   topoMatrix;    /**< TODO */
    
    /**
    * @brief  
    */
    void _init_distance_matrix();
    /**
    * @brief  
    * @param  
    * @return  
    */
    std::vector<int> _get_random_indexs(int l, int n);
    /**
    * @brief  
    * @param  
    * @return  
    */
    t_value _calculate_distance(Speaker s1, Speaker s2);
    /**
    * @brief transform form the spherical coordinate to the Cartesian coordinate
    * @param spherical spherical coordinate data structure
    * @return t_cartesian the Cartesian coordinate data
    */
    t_cartesian _spherical2cartesian(t_spherical spherical);
    /**
    * @brief transform form the Cartesian coordinate to the spherical coordinate
    * @param cartesian Cartesian coordinate data structure
    * @return t_spherical the spherical coordinate data
    */
    t_spherical _cartesian2spherical(t_cartesian cartesian);
    /**
    * @brief  
    * @param  
    * @return  
    */
    t_index _find_nearest(t_index curr_spkr, t_indexs candidates);
    /**
    * @brief  
    * @param  
    * @return  
    */
    bool _check_index_exist(t_index idx);
    /**
    * @brief  
    * @param  
    * @return  
    */
    void _setActiveSpeakerIndexs(t_indexs idxs);
    /**
    * @brief  
    * @param  
    * @return  
    */
    void _addActiveSpeakerIndexs(t_indexs idxs);
    /**
    * @brief  
    * @param  
    * @return  
    */
    void _delActiveSpeakerIndexs(t_indexs idxs);
}; 

#ifdef PUREDATA // explicit instantiation required for PD
template bool isInKey<int, Speaker>(int element, std::map<t_index, Speaker> map);
template std::string formatVector<t_index>(std::vector<t_index> vector);
#endif //PUREDATA

} //namespace zerr
#endif // SPEAKERMANAGER_H