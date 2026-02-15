#include "ringbuffer.h"
using namespace zerr;

RingBuffer::RingBuffer(size_t capacity) : buffer(capacity, 0.0f), head(0), tail(0), size(0) {}

size_t RingBuffer::get_size() const noexcept { return size; }

size_t RingBuffer::get_capacity() const noexcept { return buffer.size(); }

void RingBuffer::enqueue(const Block& block)
{
    assert(block.size() <= buffer.size() && "Block size must be smaller than buffer size.");

    for (const Sample& sample : block) {
        buffer[tail] = sample;
        tail         = (tail + 1) % buffer.size();

        if (size < buffer.size()) {
            size++;
        }
        else {
            head = (head + 1) % buffer.size();
        }
    }
}

void RingBuffer::get_samples(Sample* output_buffer, size_t buf_len)
{
    // std::cout<<"get_samples:  "<<buf_len<<" | "<<buffer.size()<<std::endl;

    assert(buf_len == buffer.size() && "Output buffer size should be equal to ringbuffer size!");

    size_t index = head;
    for (size_t i = 0; i < buffer.size(); ++i) {
        output_buffer[i] = buffer[index];
        index            = (index + 1) % buffer.size();
    }
}
