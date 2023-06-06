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

// typedef struct _signal
// {
//     int s_n;            /* number of points in the array */
//     t_sample *s_vec;    /* the array */
//     t_float s_sr;         /* sample rate */
//     int s_refcount;     /* number of times used */
//     int s_isborrowed;   /* whether we're going to borrow our array */
//     struct _signal *s_borrowedfrom;     /* signal to borrow it from */
//     struct _signal *s_nextfree;         /* next in freelist */
//     struct _signal *s_nextused;         /* next in used list */
//     int s_vecsize;      /* allocated size of array in points */
// } t_signal;

// typedef struct {
//     std::string sample_rate; /**< audio samples per second */
//     std::string block_size; /**< size of one audio block. The vector size n of the perform method might be smaller!! */
// } zerr_config;
 