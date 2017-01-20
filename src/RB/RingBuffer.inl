
#include <stdexcept>
#include <limits>

template <typename T>
RB::RingBuffer<T>::RingBuffer(std::size_t capacity) :
r(0),
w(0),
isEmpty(true)
{
    if(capacity != 0)
    {
        buffer = std::make_unique<T[]>(capacity);
    }
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
const T& RB::RingBuffer<T>::operator [](std::size_t index) const
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
const T& RB::RingBuffer<T>::at(std::size_t index) const
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
void RB::RingBuffer<T>::changeCapacity(std::size_t newCapacity)
{
    std::unique_ptr<T[]> newBuffer = std::make_unique<T[]>(newCapacity);
    if(!isEmpty)
    {
        for(std::size_t i = 0; i < getSize() && i < newCapacity; ++i)
        {
            newBuffer[i] = buffer[(r + i) % bufferSize];
        }
    }

    std::size_t size = getSize();
    r = 0;
    if(size < newCapacity)
    {
        w = size;
    }
    else
    {
        w = 0;
    }
    buffer = std::move(newBuffer);
    bufferSize = newCapacity;
}

template <typename T>
void RB::RingBuffer<T>::reserve(std::size_t newCapacity)
{
    changeCapacity(newCapacity);
}

template <typename T>
void RB::RingBuffer<T>::changeSize(std::size_t newSize)
{
    changeSize(newSize, T());
}

template <typename T>
void RB::RingBuffer<T>::changeSize(std::size_t newSize, const T& toCopy)
{
    if(newSize > bufferSize)
    {
        throw std::out_of_range("ERROR: newSize is greater than bufferSize!");
    }

    std::size_t size = getSize();
    if(newSize < size)
    {
        for(unsigned int i = 0; i < size - newSize; ++i)
        {
            if(w == 0)
            {
                w = bufferSize - 1;
            }
            else
            {
                --w;
            }
            if(w == r)
            {
                isEmpty = true;
            }
        }
    }
    else
    {
        for(unsigned int i = 0; i < newSize - size; ++i)
        {
            isEmpty = false;
            buffer[w++] = toCopy;
            if(w == bufferSize)
            {
                w = 0;
            }
        }
    }
}

template <typename T>
void RB::RingBuffer<T>::resize(std::size_t newSize)
{
    changeSize(newSize);
}

template <typename T>
void RB::RingBuffer<T>::resize(std::size_t newSize, const T& toCopy)
{
    changeSize(newSize, toCopy);
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
template <bool IsConst>
RB::RingBuffer<T>::Iterator<IsConst>::Iterator() :
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
template <bool IsConst>
RB::RingBuffer<T>::Iterator<IsConst>::Iterator(
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
template <bool IsConst>
typename RB::RingBuffer<T>::template Iterator<IsConst>::reference RB::RingBuffer<T>::Iterator<IsConst>::operator *()
{
    return buffer[index];
}

template <typename T>
template <bool IsConst>
typename RB::RingBuffer<T>::template Iterator<IsConst>& RB::RingBuffer<T>::Iterator<IsConst>::operator ++()
{
    index = (index + 1) % bufferSize;
    if(index == w)
    {
        flags.set(2);
    }
    return *this;
}

template <typename T>
template <bool IsConst>
bool RB::RingBuffer<T>::Iterator<IsConst>::operator ==(const Iterator& other) const
{
    return (flags.test(2) && other.flags.test(2))
        || (index == other.index
            && !flags.test(2) && !other.flags.test(2));
}

template <typename T>
template <bool IsConst>
bool RB::RingBuffer<T>::Iterator<IsConst>::operator !=(const Iterator& other) const
{
    return !(*this == other);
}

template <typename T>
template <bool IsConst>
typename RB::RingBuffer<T>::template Iterator<IsConst>::pointer RB::RingBuffer<T>::Iterator<IsConst>::operator ->()
{
    return &(buffer[index]);
}

template <typename T>
template <bool IsConst>
typename RB::RingBuffer<T>::template Iterator<IsConst> RB::RingBuffer<T>::Iterator<IsConst>::operator ++(int)
{
    Iterator copy = *this;
    ++(*this);
    return copy;
}

template <typename T>
template <bool IsConst>
typename RB::RingBuffer<T>::template Iterator<IsConst>& RB::RingBuffer<T>::Iterator<IsConst>::operator --()
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
template <bool IsConst>
typename RB::RingBuffer<T>::template Iterator<IsConst> RB::RingBuffer<T>::Iterator<IsConst>::operator --(int)
{
    Iterator copy = *this;
    --(*this);
    return copy;
}

template <typename T>
template <bool IsConst>
typename RB::RingBuffer<T>::template Iterator<IsConst>& RB::RingBuffer<T>::Iterator<IsConst>::operator +=(const Iterator::difference_type& n)
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
template <bool IsConst>
typename RB::RingBuffer<T>::template Iterator<IsConst> RB::RingBuffer<T>::Iterator<IsConst>::operator +(const Iterator::difference_type& n)
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
template <bool IsConst>
typename RB::RingBuffer<T>::template Iterator<IsConst>& RB::RingBuffer<T>::Iterator<IsConst>::operator -=(const Iterator::difference_type& n)
{
    return (*this) += -n;
}

template <typename T>
template <bool IsConst>
typename RB::RingBuffer<T>::template Iterator<IsConst> RB::RingBuffer<T>::Iterator<IsConst>::operator -(const Iterator::difference_type& n)
{
    Iterator copy = *this;
    return copy -= n;
}

template <typename T>
template <bool IsConst>
typename RB::RingBuffer<T>::template Iterator<IsConst>::difference_type RB::RingBuffer<T>::Iterator<IsConst>::operator -(const Iterator& other)
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
template <bool IsConst>
typename RB::RingBuffer<T>::template Iterator<IsConst>::reference RB::RingBuffer<T>::Iterator<IsConst>::operator [](const Iterator::difference_type& n)
{
    return *(*this + n);
}

template <typename T>
template <bool IsConst>
bool RB::RingBuffer<T>::Iterator<IsConst>::operator <(const Iterator& other) const
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
template <bool IsConst>
bool RB::RingBuffer<T>::Iterator<IsConst>::operator >(const Iterator& other) const
{
    return other < *this;
}

template <typename T>
template <bool IsConst>
bool RB::RingBuffer<T>::Iterator<IsConst>::operator >=(const Iterator& other) const
{
    return !(*this < other);
}

template <typename T>
template <bool IsConst>
bool RB::RingBuffer<T>::Iterator<IsConst>::operator <=(const Iterator& other) const
{
    return !(*this > other);
}

template <typename T>
typename RB::RingBuffer<T>::template Iterator<false> RB::RingBuffer<T>::begin()
{
    return Iterator<false>(r, w, bufferSize, r, isEmpty, isEmpty, buffer.get());
}

template <typename T>
typename RB::RingBuffer<T>::template Iterator<false> RB::RingBuffer<T>::end()
{
    return Iterator<false>(r, w, bufferSize, r, isEmpty, true, buffer.get());
}

template <typename T>
typename RB::RingBuffer<T>::template Iterator<true> RB::RingBuffer<T>::begin() const
{
    return cbegin();
}

template <typename T>
typename RB::RingBuffer<T>::template Iterator<true> RB::RingBuffer<T>::end() const
{
    return cend();
}

template <typename T>
typename RB::RingBuffer<T>::template Iterator<true> RB::RingBuffer<T>::cbegin() const
{
    return Iterator<true>(r, w, bufferSize, r, isEmpty, isEmpty, buffer.get());
}

template <typename T>
typename RB::RingBuffer<T>::template Iterator<true> RB::RingBuffer<T>::cend() const
{
    return Iterator<true>(r, w, bufferSize, r, isEmpty, true, buffer.get());
}

