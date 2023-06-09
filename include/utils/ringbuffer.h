/**
 * @file ringbuffer.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com) 
 * @brief ringbuffer util
 * @version 0.3
 * @date 2023-06-07
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>
#include <iostream>

#include "types.h"

namespace zerr{

class RingBuffer {
public:
    explicit RingBuffer(size_t capacity);

    size_t get_size() const;
    size_t get_capacity() const;

    void enqueue(const t_blockIn& block);
    
    void get_samples(t_sample* ptr_buffer, size_t buf_len);

private:
    t_samples buffer;
    size_t head;
    size_t tail;
    size_t size;
};

} //namespace zerr
#endif //RINGBUFFER_H
