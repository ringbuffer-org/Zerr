/**
 * @file ringbuffer.h
 * @author Zeyu Yang (zeyuuyang42@gmail.com)
 * @brief A circular buffer implementation for storing and retrieving audio samples
 * @date 2023-06-07
 *
 * @copyright Copyright (c) 2023-2024
 */
#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <cassert>
#include <iostream>
#include <vector>

#include "types.h"

namespace zerr {

/**
 * @class RingBuffer
 * @brief Implements a circular buffer for efficient sample storage and retrieval
 *
 * This class provides a fixed-size circular buffer implementation optimized for
 * audio processing applications. It allows for efficient enqueueing of sample blocks
 * and retrieval of samples in a FIFO manner.
 */
class RingBuffer {
  public:
    /**
     * @brief Construct a new Ring Buffer with specified capacity
     * @param capacity The maximum number of samples the buffer can hold
     */
    explicit RingBuffer(size_t capacity);

    /**
     * @brief Get the current number of samples in the buffer
     * @return size_t Current number of samples stored
     */
    size_t get_size() const;

    /**
     * @brief Get the maximum capacity of the buffer
     * @return size_t Maximum number of samples the buffer can hold
     */
    size_t get_capacity() const;

    /**
     * @brief Add a block of samples to the buffer
     * @param block Block of samples to enqueue
     */
    void enqueue(const Block& block);

    /**
     * @brief Retrieve samples from the buffer
     * @param ptr_buffer Pointer to destination buffer for samples
     * @param buf_len Number of samples to retrieve
     */
    void get_samples(Sample* ptr_buffer, size_t buf_len);

  private:
    Samples buffer;    ///< Storage for audio samples
    size_t head;       ///< Index where next sample will be written
    size_t tail;       ///< Index where next sample will be read
    size_t size;       ///< Current number of samples in buffer
};

}  // namespace zerr
#endif  // RINGBUFFER_H
