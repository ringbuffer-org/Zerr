#include "ringbuffer.h"
using namespace zerr;

RingBuffer::RingBuffer(size_t capacity) : buffer(capacity, 0.0f), head(0), tail(0), size(0) {}

void RingBuffer::enqueue(const t_blockIn& block) {

    assert(block.size() <= buffer.size() && "Block size must be smaller than buffer size.");

    for (const t_sample& sample : block) {
        buffer[tail] = sample;
        tail = (tail + 1) % buffer.size();

        if (size < buffer.size()) {
            size++;
        } else {
            head = (head + 1) % buffer.size();
        }
    }
}

size_t RingBuffer::getSize() const {
    return size;
}

size_t RingBuffer::getCapacity() const {
    return buffer.size();
}

t_audioBuf RingBuffer::getBufferSamples(){
    return buffer;
}