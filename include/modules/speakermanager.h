/**
 * @file speakermanager.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief  Speaker & SpeakerManager Class Implementation
 * @version 0.5
 * @date 2023-08-05
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef SPEAKERARRAY_H
#define SPEAKERARRAY_H
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
    Speaker(t_index index, t_position position, t_orientation orientation);

    // All the getters
    t_index get_index(){return index;}; 

    t_value get_x(){return position.cartesian.x;};
    t_value get_y(){return position.cartesian.y;};
    t_value get_z(){return position.cartesian.z;};

    t_value get_azimuth()  {return position.spherical.azimuth;};
    t_value get_elevation(){return position.spherical.elevation;};
    t_value get_distance() {return position.spherical.distance;};

    t_value get_yaw()  {return orientation.yaw;};
    t_value get_pitch(){return orientation.pitch;};
    
    // All the printers
    void print_all();
    void _print_index();
    void _print_position();
    void _print_orientation();

private:
    Logger *logger;

    t_index       index;
    t_position    position;
    t_orientation orientation;
} ;


/**
* SpeakerManager class is for loading speaker array configuration file 
* and accessing & choosing speakers
*/
class SpeakerManager {
public:
    SpeakerManager(std::string config_path);
    /**
    * setup speaker array via the YAML config file and specified subgroup mode
    */
    bool initialize();
    /**
    * get the number of speakers in this speaker array setup
    */
    size_t get_n_all_speakers(); 
    /**
    * get the number of speakers in this speaker array setup
    */
    size_t get_n_unmasked_speakers(); 
    /**
    *  TODO
    */
    t_indexs get_unmasked_indexs();
    /**
    *  TODO
    */
    void set_unmasked_indexs();
    /**
    *  TODO
    */
    t_index get_random_index();
    /**
    * return the speaker class acoording to speaker index
    */
    Speaker get_speaker_by_index(t_index spkr_idx);
    /**
    * get the pair of speaker indexs in sorted list acoording to the trajectory input
    */
    t_pair get_indexs_by_trajectory(t_value trajectory_val);
    /**
    * TODO
    */
    t_value get_panning_ratio(t_value trajectory_val);
    /**
    * get the index of speaker that is geometrially close to the input.
    */
    t_pair get_indexs_by_geometry(std::vector<t_value> pos, std::vector<bool> mask, std::string coordinate);
    /**
    * jump between different speakers using trigger signal
    * the 
    */
    t_index get_indexs_by_trigger(t_value trigger, t_index curr_spkr, int mode);
    /**
    *  TODO
    */
    std::vector<float> get_distance_vector(int spkr_idx);

    /**
    *  TODO
    */
    void set_subindex();
    /**
    *  TODO
    */
    void set_topology_matrix();
    /**
    *  TODO
    */
    void reset(std::string config_path);

private:
    Logger *logger;         /**< logger object for logging message in different environment */

    bool initialized=false; /**< indicator of whether the speaker manager object has been initialized or not */

    std::string config_path;   /**< file path of speaker array configuration */
    YAML::Node speaker_config; /**< loaded YAML Node structure of speaker array configuration */
    
    std::map<t_index, Speaker> speakers; /**< TODO  */

    std::map<t_index, std::vector<t_value>> distance_matrix; /**< TODO */

    t_indexs unmasked;                           /**< TODO */
    t_indexs subindex;                           /**< TODO */
    std::map<t_index, t_indexs> topology_matrix; /**< TODO */
    
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
    
}; 

} //namespace zerr
#endif // SPEAKERARRAY_H