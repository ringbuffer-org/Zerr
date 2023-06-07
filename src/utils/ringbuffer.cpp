#include "ringbuffer.h"
// using namespace zerr;

// template<typename T>
RingBuffer::RingBuffer(size_t capacity) : buffer(capacity, 0.0f), head(0), tail(0), size(0) {}

// template<typename T>
// void RingBuffer::enqueueBlock(const std::vector<float>& block) {
//     if (block.size() > buffer.size() - size) {
//         std::cerr << "Insufficient space in the buffer to enqueue the block." << std::endl;
//         return;
//     }

//     for (const float& item : block) {
//         enqueue(item);
//     }
// }

void RingBuffer::enqueue(const float& item) {
    if (size == buffer.size()) {
        std::cerr << "Buffer is full. Unable to enqueue." << std::endl;
        return;
    }

    buffer[tail] = item;
    tail = (tail + 1) % buffer.size();
    size++;
}

// template<typename T>
std::vector<float> RingBuffer::dequeue(size_t blockSize) {
    if (blockSize > size) {
        std::cerr << "Insufficient items in the buffer to dequeue the block." << std::endl;
        return std::vector<float>();
    }

    std::vector<float> items;
    items.reserve(blockSize);

    for (size_t i = 0; i < blockSize; ++i) {
        items.push_back(buffer[head]);
        head = (head + 1) % buffer.size();
        size--;
    }

    return items;
}

// template<typename T>
size_t RingBuffer::getSize() const {
    return size;
}

// template<typename T>
size_t RingBuffer::getCapacity() const {
    return buffer.size();
}

std::vector<float> RingBuffer::getBufferSamples(){
    return buffer;
}