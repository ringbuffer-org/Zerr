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

// #ifndef PUREDATA
// #define PUREDATA

// #include <stddef.h>
// #include <string>

// #ifdef __cplusplus
// extern "C" {
// #endif
/**
 * @struct sys_config
 * @brief Puredata system configuration structure for initializing the zerr engine.
 */
typedef struct {
    int sample_rate; /**< audio samples per second */
    int block_size; /**< size of one audio block. The vector size n of the perform method might be smaller!! */
} sys_config;

class SystemConfig{
public:
    SystemConfig(int sr, int bs){sample_rate=sr;block_size=bs;}
    int sample_rate; 
    int block_size;
};

// typedef struct {
//     std::string sample_rate; /**< audio samples per second */
//     std::string block_size; /**< size of one audio block. The vector size n of the perform method might be smaller!! */
// } zerr_config;

// #ifdef __cplusplus
// }
// #endif