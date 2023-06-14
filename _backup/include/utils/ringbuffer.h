/**
 * @file ringbuffer.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com) & ChatGPT
 * @brief ringbuffer util
 * @version 0.3
 * @date 2023-06-07
 * 
 * @copyright Copyright (c) 2023
 */
#pragma once

#include <vector>
#include <iostream>

// namespace zerr{

// template<typename T>
class RingBuffer {
public:
    explicit RingBuffer(size_t capacity);

    // void enqueue(const T& item);
    // void enqueueBlock(const std::vector<float>& block);
    void enqueue(const float& item); 

    // std::vector<T> dequeue();
    std::vector<float> dequeue(size_t blockSize);

    size_t getSize() const;
    size_t getCapacity() const;
    std::vector<float> getBufferSamples();

private:
    std::vector<float> buffer;
    size_t head;
    size_t tail;
    size_t size;
};

// } //namespace zerr
