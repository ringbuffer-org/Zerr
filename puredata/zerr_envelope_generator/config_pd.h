/**
 * @file config_pd.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief Zerr Puredata Config File
 * @version 0.3
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 */
#pragma once
#include <stddef.h>


typedef struct {
    size_t sample_rate;
    size_t block_size;
}t_systemConfigs;