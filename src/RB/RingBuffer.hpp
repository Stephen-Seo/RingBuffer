
#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#define RING_BUFFER_DEFAULT_CAPACITY 1024

#include <cstdlib>
#include <cstddef>

#include <memory>
#include <bitset>
#include <type_traits>

namespace RB
{

template <typename T>
class RingBuffer
{
public:
    typedef T value_type;

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

    void changeCapacity(std::size_t newCapacity);
    void reserve(std::size_t newCapacity);

    void changeSize(std::size_t newSize);
    void changeSize(std::size_t newSize, const T& toCopy);
    void resize(std::size_t newSize);
    void resize(std::size_t newSize, const T& toCopy);

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
    template <bool IsConst>
    class Iterator
    {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef std::conditional_t<IsConst, const T&, T&> reference;
        typedef std::conditional_t<IsConst, const T*, T*> pointer;
        typedef std::random_access_iterator_tag iterator_category;

        typedef RingBuffer<T> parent_type;

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

    Iterator<false> begin();
    Iterator<false> end();

    Iterator<true> begin() const;
    Iterator<true> end() const;

    Iterator<true> cbegin() const;
    Iterator<true> cend() const;

};

} // namespace RB

#include "RingBuffer.inl"

#endif

