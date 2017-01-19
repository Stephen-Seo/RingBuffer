
#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#define RING_BUFFER_DEFAULT_CAPACITY 1024

#include <cstdlib>
#include <cstddef>

#include <memory>
#include <bitset>

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

    T& operator [](std::size_t index);
    T& at(std::size_t index);

    bool empty() const;
    std::size_t getCapacity() const;
    std::size_t getSize() const;

private:
    std::size_t r;
    std::size_t w;
    std::size_t bufferSize;
    bool isEmpty;
    std::unique_ptr<T[]> buffer;

    void checkPush() const;
    void checkPop() const;
    void copyRingBuffer(const RingBuffer<T>& other);

public:
    class Iterator
    {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef std::random_access_iterator_tag iterator_category;

        Iterator();
        Iterator(
            const std::size_t& r,
            const std::size_t& w,
            const std::size_t& bufferSize,
            const std::size_t& index,
            bool isEmpty,
            bool isEnd,
            pointer buffer
        );

        // copy
        Iterator(const Iterator& other) = default;
        Iterator& operator =(const Iterator& other) = default;

        // move
        Iterator(Iterator&& other) = default;
        Iterator& operator =(Iterator&& other) = default;

        reference operator *();
        Iterator& operator ++();

        bool operator ==(const Iterator& other) const;

        bool operator !=(const Iterator& other) const;
        pointer operator ->();
        Iterator operator ++(int);

        Iterator& operator --();
        Iterator operator --(int);

        Iterator& operator +=(const difference_type& n);
        Iterator operator +(const difference_type& n);
        Iterator& operator -=(const difference_type& n);
        Iterator operator -(const difference_type& n);
        difference_type operator -(const Iterator& other);
        reference operator [](const difference_type& n);
        bool operator <(const Iterator& other) const;
        bool operator >(const Iterator& other) const;
        bool operator >=(const Iterator& other) const;
        bool operator <=(const Iterator& other) const;

    private:
        std::size_t r;
        std::size_t w;
        std::size_t bufferSize;
        std::size_t index;
        // 0 - isEmpty
        // 1 - initialized
        // 2 - past the end reached
        std::bitset<3> flags;
        pointer buffer;

    };

    Iterator begin();
    Iterator end();

};

} // namespace RB

#include "RingBuffer.inl"

#endif

