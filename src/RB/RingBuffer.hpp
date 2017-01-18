
#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#define RING_BUFFER_DEFAULT_CAPACITY 1024

#include <cstdlib>

#include <memory>

namespace RB
{

template <typename T>
class RingBuffer
{
public:
    RingBuffer(std::size_t capacity = RING_BUFFER_DEFAULT_CAPACITY);

    // copy
    RingBuffer(const RingBuffer<T>& other);
    RingBuffer<T>& operator=(const RingBuffer<T>& other);

    // move
    RingBuffer(RingBuffer<T>&& other) = default;
    RingBuffer<T>& operator=(RingBuffer<T>&& other) = default;

    void push(const T& reference);
    void push(T&& r_value);
    T&& pop();

    // TODO indexing

    bool empty() const;
    std::size_t getCapacity() const;
    std::size_t getSize() const;

private:
    std::size_t r;
    std::size_t w;
    std::size_t bufferSize;
    std::unique_ptr<T[]> buffer;

    void checkOverflow() const;
    void checkUnderflow() const;
    void copyRingBuffer(const RingBuffer<T>& other);

};

#include "RingBuffer.inl"

} // namespace RB

#endif

