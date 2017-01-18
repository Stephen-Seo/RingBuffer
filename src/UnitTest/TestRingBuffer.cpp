
#include <stdexcept>

#include "gtest/gtest.h"

#include <RB/RingBuffer.hpp>

using namespace RB;

TEST(RingBuffer, PopPush)
{
    RingBuffer<int> rb(5);

    for(int i = 0; i < 5; ++i)
    {
        rb.push(i);
    }

    bool exceptionThrown = false;
    try
    {
        rb.push(5);
    }
    catch (const std::out_of_range& e)
    {
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown);

    for(int i = 0; i < 5; ++i)
    {
        rb.pop();
    }

    exceptionThrown = false;
    try
    {
        rb.pop();
    }
    catch (const std::out_of_range& e)
    {
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown);

    rb.push(0);
    rb.push(1);
    rb.pop();
    rb.pop();

    for(int i = 0; i < 5; ++i)
    {
        rb.push(i);
    }

    exceptionThrown = false;
    try
    {
        rb.push(5);
    }
    catch (const std::out_of_range& e)
    {
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown);

    for(int i = 0; i < 5; ++i)
    {
        rb.pop();
    }

    exceptionThrown = false;
    try
    {
        rb.pop();
    }
    catch (const std::out_of_range& e)
    {
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown);
}

TEST(RingBuffer, Capacity)
{
    RingBuffer<char> rb(0);
    EXPECT_EQ(RING_BUFFER_DEFAULT_CAPACITY, rb.getCapacity());

    rb = RingBuffer<char>(512);
    EXPECT_EQ(512, rb.getCapacity());
}

TEST(RingBuffer, Empty)
{
    RingBuffer<char> rb(5);

    EXPECT_TRUE(rb.empty());

    for(int i = 0; i < 3; ++i)
    {
        rb.push('a');
        EXPECT_FALSE(rb.empty());
    }

    for(int i = 0; i < 3; ++i)
    {
        EXPECT_FALSE(rb.empty());
        rb.pop();
    }

    EXPECT_TRUE(rb.empty());

    for(int i = 0; i < 5; ++i)
    {
        rb.push('b');
    }

    EXPECT_FALSE(rb.empty());

    for(int i = 0; i < 5; ++i)
    {
        rb.pop();
    }

    EXPECT_TRUE(rb.empty());
}

TEST(RingBuffer, GetSize)
{
    RingBuffer<char> rb(20);

    EXPECT_EQ(0, rb.getSize());

    for(unsigned int i = 0; i < 5; ++i)
    {
        rb.push('b');
        EXPECT_EQ(i + 1, rb.getSize());
    }

    for(unsigned int i = 0; i < 5; ++i)
    {
        rb.pop();
        EXPECT_EQ(4 - i, rb.getSize());
    }

    for(unsigned int i = 0; i < 20; ++i)
    {
        rb.push('a');
        EXPECT_EQ(i + 1, rb.getSize());
    }

    for(unsigned int i = 0; i < 20; ++i)
    {
        rb.pop();
        EXPECT_EQ(19 - i, rb.getSize());
    }
}

TEST(RingBuffer, Indexing)
{
    RingBuffer<char> rb(10);

    for(std::size_t i = 0; i < 3; ++i)
    {
        rb.push('a' + i);
    }

    EXPECT_EQ('a', rb.at(0));
    EXPECT_EQ('b', rb.at(1));
    EXPECT_EQ('c', rb.at(2));

    bool exceptionThrown = false;
    try
    {
        rb.at(3);
    }
    catch (const std::out_of_range& e)
    {
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown);

    for(std::size_t i = 0; i < 3; ++i)
    {
        rb.pop();
    }

    exceptionThrown = false;
    try
    {
        rb.at(0);
    }
    catch (const std::out_of_range& e)
    {
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown);

    for(std::size_t i = 0; i < 10; ++i)
    {
        rb.push('a' + i);
    }

    EXPECT_EQ('a', rb.at(0));
    EXPECT_EQ('b', rb.at(1));
    EXPECT_EQ('c', rb.at(2));
    EXPECT_EQ('d', rb.at(3));
    EXPECT_EQ('e', rb.at(4));
    EXPECT_EQ('f', rb.at(5));
    EXPECT_EQ('g', rb.at(6));
    EXPECT_EQ('h', rb.at(7));
    EXPECT_EQ('i', rb.at(8));
    EXPECT_EQ('j', rb.at(9));

    exceptionThrown = false;
    try
    {
        rb.at(10);
    }
    catch (const std::out_of_range& e)
    {
        exceptionThrown = true;
    }
    EXPECT_TRUE(exceptionThrown);
}

