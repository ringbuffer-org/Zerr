/**
 * @file zerr_envelope_generator.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Envelope Generator Class Puredata Wrapper
 * @version 0.3
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 */
#pragma once


// dependencies
#include "utils.h"
#include "types.h"
#include "ringbuffer.h"

// modules
#include "mapper.h"

#include "m_pd.h" // for testing


class ZerrEnvelopeGenerator{
public:
    int n_outlet;
    int n_inlet=3;
    /**
    * create a new zerr_speaker_mapper instance 
    */
    ZerrEnvelopeGenerator(zerr::t_systemConfigs sys_cnfg, std::string spkrCfgFile);
    /**
    * initialize all zerr_speaker_mapper modules
    */
    void initialize();
    /**
    * callback function
    */
    // void perform(float **in, float **out, int n_vec);
    /**
    * process a block of samples
    */
    void perform(float **ports, int n_vec);
    /**
    * return the total number of inlet plus outlet
    * 
    */
    int get_port_count();
    /**
    * free a zerr_speaker_mapper instance
    */
    ~ZerrEnvelopeGenerator();

private:
    //basic config
    zerr::t_systemConfigs system_configs;

    std::vector<std::vector<double>> input_buffer;
    std::vector<std::vector<double>> output_buffer;
    float **in_ptr;
    float **out_ptr;

    // config path
    std::string zerr_cfg;
    std::string spkr_cfg;

    //module objects
    zerr::Mapper              *mapper;
};





