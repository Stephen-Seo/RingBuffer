
#include <iostream>
#include <stdexcept>
#include <limits>
#include <type_traits>

template <typename T>
RB::RingBuffer<T>::RingBuffer(std::size_t capacity) :
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
RB::RingBuffer<T>::RingBuffer(const RB::RingBuffer<T>& other) :
r(0),
w(0)
{
    copyRingBuffer(other);
}

template <typename T>
RB::RingBuffer<T>& RB::RingBuffer<T>::operator =(const RB::RingBuffer<T>& other)
{
    copyRingBuffer(other);
    return *this;
}

template <typename T>
void RB::RingBuffer<T>::push(const T& reference)
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
void RB::RingBuffer<T>::push(T&& r_value)
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
T&& RB::RingBuffer<T>::pop()
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
T& RB::RingBuffer<T>::operator [](std::size_t index)
{
    return buffer[(index + r) % bufferSize];
}

template <typename T>
T& RB::RingBuffer<T>::at(std::size_t index)
{
    if(index >= getSize())
    {
        throw std::out_of_range("ERROR: Index is too large!");
    }

    return (*this)[index];
}

template <typename T>
bool RB::RingBuffer<T>::empty() const
{
    return isEmpty;
}

template <typename T>
std::size_t RB::RingBuffer<T>::getCapacity() const
{
    return bufferSize;
}

template <typename T>
std::size_t RB::RingBuffer<T>::getSize() const
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
void RB::RingBuffer<T>::checkPush() const
{
    if(!isEmpty && r == w)
    {
        throw std::out_of_range("RingBuffer max capacity reached, cannot push!");
    }
}

template <typename T>
void RB::RingBuffer<T>::checkPop() const
{
    if(isEmpty)
    {
        throw std::out_of_range("RingBuffer is empty, cannot pop!");
    }
}

template <typename T>
void RB::RingBuffer<T>::copyRingBuffer(const RB::RingBuffer<T>& other)
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

template <typename T>
RB::RingBuffer<T>::Iterator::Iterator() :
r(0),
w(0),
bufferSize(0),
index(0),
flags(),
buffer(nullptr)
{
    flags.set(0);
}

template <typename T>
RB::RingBuffer<T>::Iterator::Iterator(
    const std::size_t& r,
    const std::size_t& w,
    const std::size_t& bufferSize,
    const std::size_t& index,
    bool isEmpty,
    bool isEnd,
    pointer buffer
) :
r(r),
w(w),
bufferSize(bufferSize),
index(index),
flags(),
buffer(buffer)
{
    flags.set(0, isEmpty);
    flags.set(1);
    flags.set(2, isEnd);
}

template <typename T>
typename RB::RingBuffer<T>::Iterator::reference RB::RingBuffer<T>::Iterator::operator *()
{
    return buffer[index];
}

template <typename T>
typename RB::RingBuffer<T>::Iterator& RB::RingBuffer<T>::Iterator::operator ++()
{
    index = (index + 1) % bufferSize;
    if(index == w)
    {
        flags.set(2);
    }
    return *this;
}

template <typename T>
bool RB::RingBuffer<T>::Iterator::operator ==(const Iterator& other) const
{
    return (flags.test(2) && other.flags.test(2))
        || (index == other.index
            && !flags.test(2) && !other.flags.test(2));
}

template <typename T>
bool RB::RingBuffer<T>::Iterator::operator !=(const Iterator& other) const
{
    return !(*this == other);
}

template <typename T>
typename RB::RingBuffer<T>::Iterator::pointer RB::RingBuffer<T>::Iterator::operator ->()
{
    return &(buffer[index]);
}

template <typename T>
typename RB::RingBuffer<T>::Iterator RB::RingBuffer<T>::Iterator::operator ++(int)
{
    Iterator copy = *this;
    ++(*this);
    return copy;
}

template <typename T>
typename RB::RingBuffer<T>::Iterator& RB::RingBuffer<T>::Iterator::operator --()
{
    if(flags.test(2))
    {
        if(w == 0)
        {
            index = bufferSize - 1;
        }
        else
        {
            index = w - 1;
        }

        flags.reset(2);
    }
    else
    {
        if(index == 0)
        {
            index = bufferSize - 1;
        }
        else
        {
            --index;
        }
    }

    return *this;
}

template <typename T>
typename RB::RingBuffer<T>::Iterator RB::RingBuffer<T>::Iterator::operator --(int)
{
    Iterator copy = *this;
    --(*this);
    return copy;
}

template <typename T>
typename RB::RingBuffer<T>::Iterator& RB::RingBuffer<T>::Iterator::operator +=(const RB::RingBuffer<T>::Iterator::difference_type& n)
{
    difference_type m = n;
    if(m >= 0)
    {
        while(m--)
        {
            ++(*this);
        }
    }
    else
    {
        while(m++)
        {
            --(*this);
        }
    }

    return *this;
}

template <typename T>
typename RB::RingBuffer<T>::Iterator RB::RingBuffer<T>::Iterator::operator +(const RB::RingBuffer<T>::Iterator::difference_type& n)
{
    Iterator copy = *this;
    return copy += n;
}

// only enable for ContainerType == RB::RingBuffer<T>
template <
    typename IteratorType,
    typename ContainerType = typename IteratorType::parent_type,
    typename T = typename ContainerType::value_type,
    typename = std::enable_if_t<std::is_same<ContainerType, RB::RingBuffer<T>>::value>
>
IteratorType operator +(const typename IteratorType::difference_type& n, const IteratorType& iter)
{
    IteratorType copy = iter;
    return copy += n;
}

template <typename T>
typename RB::RingBuffer<T>::Iterator& RB::RingBuffer<T>::Iterator::operator -=(const RB::RingBuffer<T>::Iterator::difference_type& n)
{
    return (*this) += -n;
}

template <typename T>
typename RB::RingBuffer<T>::Iterator RB::RingBuffer<T>::Iterator::operator -(const RB::RingBuffer<T>::Iterator::difference_type& n)
{
    Iterator copy = *this;
    return copy -= n;
}

template <typename T>
typename RB::RingBuffer<T>::Iterator::difference_type RB::RingBuffer<T>::Iterator::operator -(const typename RB::RingBuffer<T>::Iterator& other)
{
    if(flags.test(2) && other.flags.test(2))
    {
        return 0;
    }
    else if(flags.test(2))
    {
        if(w >= other.index)
        {
            return w - other.index;
        }
        else
        {
            return w + other.bufferSize - other.index;
        }
    }
    else if(!flags.test(2) && !other.flags.test(2))
    {
        if(index >= other.index)
        {
            return index - other.index;
        }
        else
        {
            return index + other.bufferSize - other.index;
        }
    }
    else
    {
        // invalid condition
        return 0;
    }
}

template <typename T>
typename RB::RingBuffer<T>::Iterator::reference RB::RingBuffer<T>::Iterator::operator [](const typename RB::RingBuffer<T>::Iterator::difference_type& n)
{
    return *(*this + n);
}

template <typename T>
bool RB::RingBuffer<T>::Iterator::operator <(const typename RB::RingBuffer<T>::Iterator& other) const
{
    if(flags.test(2))
    {
        return false;
    }
    else if(other.flags.test(2))
    {
        return true;
    }
    else
    {
        if(index >= r && other.index >= other.r)
        {
            return index < other.index;
        }
        else if(index >= r && other.index < other.r)
        {
            return true;
        }
        else if(index < r && other.index >= other.r)
        {
            return false;
        }
        else //if(index < r && other.index < other.r)
        {
            return index < other.index;
        }
    }
}

template <typename T>
bool RB::RingBuffer<T>::Iterator::operator >(const typename RB::RingBuffer<T>::Iterator& other) const
{
    return other < *this;
}

template <typename T>
bool RB::RingBuffer<T>::Iterator::operator >=(const typename RB::RingBuffer<T>::Iterator& other) const
{
    return !(*this < other);
}

template <typename T>
bool RB::RingBuffer<T>::Iterator::operator <=(const typename RB::RingBuffer<T>::Iterator& other) const
{
    return !(*this > other);
}

template <typename T>
typename RB::RingBuffer<T>::Iterator RB::RingBuffer<T>::begin()
{
    return Iterator(r, w, bufferSize, r, isEmpty, isEmpty, buffer.get());
}

template <typename T>
typename RB::RingBuffer<T>::Iterator RB::RingBuffer<T>::end()
{
    return Iterator(r, w, bufferSize, r, isEmpty, true, buffer.get());
}

