/**
 * @file zerr_config.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Zerr Puredata Config File
 * @version 0.3
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 */
#pragma once

// #ifndef CPLUSPLUS
#define CPLUSPLUS

// #ifndef PUREDATA
#define PUREDATA

#include <stddef.h>

/**
 * @struct zerr_config
 * @brief Configuration structure for the zerr engine.
 */
typedef struct {
    int sample_rate; /**< audio samples per second */
    int block_size; /**< size of one audio block. The vector size n of the perform method might be smaller!! */
    // control_manager *mgr; /**< global control manager */
} sys_config;