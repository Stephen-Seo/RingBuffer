
#include <iostream>
#include <stdexcept>
#include <limits>

template <typename T>
RingBuffer<T>::RingBuffer(std::size_t capacity) :
r(0),
w(0),
isEmpty(true)
{
    if(capacity == 0)
    {
        std::cerr << "WARNING: Invalid capacity of " << capacity
            << ", using default of " << RING_BUFFER_DEFAULT_CAPACITY
            << std::endl;
        capacity = RING_BUFFER_DEFAULT_CAPACITY;
    }

    buffer = std::make_unique<T[]>(capacity);
    bufferSize = capacity;
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
//    std::clog << "RingBuffer<T>::push(const T&) called" << std::endl;
#endif
    checkPush();

    T value = reference;

    buffer[w] = std::move(value);
    w = (w + 1) % bufferSize;

    isEmpty = false;
}

template <typename T>
void RingBuffer<T>::push(T&& r_value)
{
#ifndef NDEBUG
//    std::clog << "RingBuffer<T>::push(T&&) called" << std::endl;
#endif
    checkPush();

    buffer[w] = std::forward<T>(r_value);
    w = (w + 1) % bufferSize;

    isEmpty = false;
}

template <typename T>
T&& RingBuffer<T>::pop()
{
    checkPop();

    T value = std::move(buffer[r]);
    r = (r + 1) % bufferSize;

    if(r == w)
    {
        isEmpty = true;
    }

    return std::move(value);
}

template <typename T>
T& RingBuffer<T>::operator [](std::size_t index)
{
    return buffer[(index + r) % bufferSize];
}

template <typename T>
T& RingBuffer<T>::at(std::size_t index)
{
    if(index >= getSize())
    {
        throw std::out_of_range("ERROR: Index is too large!");
    }

    return (*this)[index];
}

template <typename T>
bool RingBuffer<T>::empty() const
{
    return isEmpty;
}

template <typename T>
std::size_t RingBuffer<T>::getCapacity() const
{
    return bufferSize;
}

template <typename T>
std::size_t RingBuffer<T>::getSize() const
{
    if(isEmpty)
    {
        return 0;
    }
    else
    {
        return r < w ? w - r : bufferSize - r + w;
    }
}

template <typename T>
void RingBuffer<T>::checkPush() const
{
    if(!isEmpty && r == w)
    {
        throw std::out_of_range("RingBuffer max capacity reached, cannot push!");
    }
}

template <typename T>
void RingBuffer<T>::checkPop() const
{
    if(isEmpty)
    {
        throw std::out_of_range("RingBuffer is empty, cannot pop!");
    }
}

template <typename T>
void RingBuffer<T>::copyRingBuffer(const RingBuffer<T>& other)
{
    r = 0;
    w = 0;
    isEmpty = true;
    buffer = std::make_unique<T[]>(other.bufferSize);
    bufferSize = other.bufferSize;
    if(other.buffer && !other.isEmpty)
    {
        isEmpty = false;
        for(std::size_t i = other.r;
            i != other.w;
            i = (i + 1) % bufferSize)
        {
            buffer[w] = other.buffer[i];
            w = w + 1; // shouldn't need to use modulo
        }
    }
}

