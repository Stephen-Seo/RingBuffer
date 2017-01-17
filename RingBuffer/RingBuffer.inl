
#include <iostream>
#include <stdexcept>
#include <limits>

template <typename T>
RingBuffer<T>::RingBuffer(std::size_t capacity) :
r(0),
w(0)
{
    if(capacity == 0)
    {
        std::cerr << "WARNING: Invalid capacity of " << capacity
            << ", using default of " << RING_BUFFER_DEFAULT_CAPACITY
            << std::endl;
        capacity = RING_BUFFER_DEFAULT_CAPACITY;
    }
    else if(capacity == std::numeric_limits<std::size_t>::max())
    {
        std::cerr << "WARNING: Capacity cannot be max ("
            << std::numeric_limits<std::size_t>::max()
            << ") due to limitation with RingBuffer, setting capacity to "
            << std::numeric_limits<std::size_t>::max() - 1
            << std::endl;
        capacity = std::numeric_limits<std::size_t>::max() - 1;
    }

    buffer = std::make_unique<T[]>(capacity + 1);
    bufferSize = capacity + 1;
}

template <typename T>
RingBuffer<T>::RingBuffer(const RingBuffer<T>& other) :
r(0),
w(0)
{
    copyRingBuffer(other);
}

template <typename T>
RingBuffer<T>& RingBuffer<T>::operator =(const RingBuffer<T>& other)
{
    copyRingBuffer(other);
    return *this;
}

template <typename T>
void RingBuffer<T>::push(const T& reference)
{
#ifndef NDEBUG
    std::clog << "RingBuffer<T>::push(const T&) called" << std::endl;
#endif
    checkOverflow();

    T value = reference;

    buffer[w] = std::move(value);
    w = (w + 1) % bufferSize;
}

template <typename T>
void RingBuffer<T>::push(T&& r_value)
{
#ifndef NDEBUG
    std::clog << "RingBuffer<T>::push(T&&) called" << std::endl;
#endif
    checkOverflow();

    buffer[w] = std::forward<T>(r_value);
    w = (w + 1) % bufferSize;
}

template <typename T>
T&& RingBuffer<T>::pop()
{
    checkUnderflow();

    T value = std::move(buffer[r]);
    r = (r + 1) % bufferSize;

    return std::move(value);
}

template <typename T>
bool RingBuffer<T>::empty() const
{
    return r == w;
}

template <typename T>
std::size_t RingBuffer<T>::getCapacity() const
{
    return bufferSize - 1;
}

template <typename T>
std::size_t RingBuffer<T>::getSize() const
{
    if(empty())
    {
        return 0;
    }
    else
    {
        return r < w ? w - r : bufferSize - r + w;
    }
}

template <typename T>
void RingBuffer<T>::checkOverflow() const
{
    if((w == std::numeric_limits<std::size_t>::max() && r == 0)
        || (w == bufferSize - 1 && r == 0)
        || (w + 1 == r))
    {
        throw std::overflow_error("RingBuffer max capacity reached, cannot push!");
    }
}

template <typename T>
void RingBuffer<T>::checkUnderflow() const
{
    if(r == w)
    {
        throw std::underflow_error("RingBuffer is empty, cannot pop!");
    }
}

template <typename T>
void RingBuffer<T>::copyRingBuffer(const RingBuffer<T>& other)
{
    r = 0;
    w = 0;
    buffer = std::make_unique<T[]>(other.bufferSize);
    bufferSize = other.bufferSize;
    if(other.buffer && !other.empty())
    {
        for(std::size_t i = other.r;
            i != other.w;
            i = (i + 1) % bufferSize)
        {
            buffer[w] = other.buffer[i];
            w = w + 1; // shouldn't need to use modulus
        }
    }
}

