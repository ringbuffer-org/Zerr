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
#include <stddef.h>

/**
 * @struct sys_config
 * @brief Puredata system configuration structure for initializing the zerr engine.
 */
typedef struct {
    size_t sample_rate;
    size_t block_size;
}t_systemConfigs;


typedef struct {
    char **names;  // Pointer to the list of strings
    int num;     // Number of strings in the list
} t_featureNames;