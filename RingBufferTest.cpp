
#include <iostream>
#include <stdexcept>
#include <string>

#include "RingBuffer/RingBuffer.hpp"

int main()
{
    RingBuffer<std::size_t> ringBuffer(13);

    if(!ringBuffer.empty())
    {
        throw std::logic_error("ERROR: Empty RingBuffer returned not empty!");
    }
    else if(ringBuffer.getCapacity() != 13)
    {
        throw std::logic_error("ERROR: RingBuffer returned wrong capacity!"
            " Got " + std::to_string(ringBuffer.getCapacity())
            + " instead of 13!");
    }

    for(std::size_t i = 0; i < 10; ++i)
    {
        std::cout << "Inserting " << i << std::endl;
        ringBuffer.push(i);
    }

    std::cout << "Move inserting 10" << std::endl;
    ringBuffer.push(10);

    std::cout << "Not move inserting 11" << std::endl;
    {
        std::size_t value = 11;
        ringBuffer.push(value);
    }

    std::cout << "Move inserting 12" << std::endl;
    {
        std::size_t value = 12;
        ringBuffer.push(std::move(value));
    }

    std::cout << "Test overflow (push)" << std::endl;
    try
    {
        ringBuffer.push(13);
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    std::cout << "Test copy" << std::endl;
    {
        RingBuffer<std::size_t> other(ringBuffer);
        if(other.getSize() != 13 || other.getCapacity() != 13)
        {
            throw std::logic_error("ERROR: RingBuffer failed to copy properly!"
                " Copied size is " + std::to_string(other.getSize())
                + ", and copied capacity is " + std::to_string(other.getCapacity()));
        }
    }

    for(std::size_t i = 0; i <= 12; ++i)
    {
        std::size_t value = ringBuffer.pop();
        std::cout << "Popped, got " << value << std::endl;
    }

    std::cout << "Test underflow (pop)" << std::endl;
    try
    {
        ringBuffer.pop();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    if(!ringBuffer.empty())
    {
        throw std::logic_error("ERROR: Empty RingBuffer returned not empty!");
    }

    std::cout << "Testing size 1 ring buffer" << std::endl;
    ringBuffer = RingBuffer<std::size_t>(1);

    if(ringBuffer.getCapacity() != 1)
    {
        throw std::logic_error("ERROR: RingBuffer returned wrong capacity!"
            " Got " + std::to_string(ringBuffer.getCapacity())
            + " instead of 1!");
    }

    ringBuffer.push(42);

    try
    {
        ringBuffer.push(0);
    }
    catch(const std::exception& e)
    {}

    ringBuffer.pop();

    try
    {
        ringBuffer.pop();
    }
    catch(const std::exception& e)
    {}

    std::cout << "Size 1 ring buffer SUCCESS" << std::endl;

    std::cout << "Testing ring buffer's ring property" << std::endl;
    ringBuffer = RingBuffer<std::size_t>(10);

    for(std::size_t i = 0; i < 5; ++i)
    {
        ringBuffer.push(i);
    }
    for(std::size_t i = 0; i < 5; ++i)
    {
        ringBuffer.pop();
    }

    if(ringBuffer.getSize() != 0)
    {
        throw std::logic_error("ERROR: Wrong size! Should be 0, got "
            + std::to_string(ringBuffer.getSize()));
    }

    for(std::size_t i = 0; i < 10; ++i)
    {
        ringBuffer.push(i);
        if(ringBuffer.getSize() != i + 1)
        {
            throw std::logic_error("ERROR: Wrong size! Should be "
                + std::to_string(i + 1) + ", got "
                + std::to_string(ringBuffer.getSize()));
        }
    }
    try
    {
        ringBuffer.push(42);
    }
    catch(const std::exception& e)
    {
        std::cout << "Exception caught (overflow)" << std::endl;
    }

    for(std::size_t i = 0; i < 10; ++i)
    {
        ringBuffer.pop();
        if(ringBuffer.getSize() != 10 - (i + 1))
        {
            throw std::logic_error("ERROR: Wrong size! Should be "
                + std::to_string(10 - (i + 1)) + ", got "
                + std::to_string(ringBuffer.getSize()));
        }
    }
    try
    {
        ringBuffer.pop();
    }
    catch(const std::exception& e)
    {
        std::cout << "Exception caught (underflow)" << std::endl;
    }

    std::cout << "Testing ring property of ring buffer SUCCESS" << std::endl;

    std::cout << "COMPLETE SUCCESS! (Why aren't you using GTest unit tests "
        "you silly person?)" << std::endl;
}

