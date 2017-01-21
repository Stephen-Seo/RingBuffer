
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
    EXPECT_EQ(0, rb.getCapacity());

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

TEST(RingBuffer, Iterator)
{
    RingBuffer<char> rb(10);

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.push('a' + i);
    }

    {
        auto iterator = rb.begin();
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(*iterator, 'a' + i);
            ++iterator;
        }
        EXPECT_EQ(iterator, rb.end());
    }

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.pop();
        rb.push('a' + i);
    }

    {
        auto iterator = rb.begin();
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(*iterator, 'a' + i);
            ++iterator;
        }
        EXPECT_EQ(iterator, rb.end());
    }
}

TEST(RingBuffer, ConstIterator)
{
    RingBuffer<char> rb(10);

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.push('a' + i);
    }

    {
        auto iterator = rb.cbegin();
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(*iterator, 'a' + i);
            ++iterator;
        }
        EXPECT_EQ(iterator, rb.cend());
    }

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.pop();
        rb.push('a' + i);
    }

    {
        auto iterator = rb.cbegin();
        EXPECT_TRUE(std::is_const<std::remove_reference_t<decltype(*iterator)>>::value);
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(*iterator, 'a' + i);
            ++iterator;
        }
        EXPECT_EQ(iterator, rb.cend());
    }
}

TEST(RingBuffer, OutputIterator)
{
    RingBuffer<char> rb(10);

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.push('a' + i);
    }

    {
        auto iterator = rb.begin();
        EXPECT_EQ(*iterator, 'a');

        *iterator = '0';
        EXPECT_EQ(*iterator, '0');

        {
            decltype(iterator)& incIter = ++iterator;
            EXPECT_EQ(incIter, iterator);
        }
        {
            const decltype(iterator)& incIter = iterator++;
            EXPECT_NE(incIter, iterator);
        }

        iterator = rb.begin();
        *iterator++ = '1';
        EXPECT_EQ(*rb.begin(), '1');

        *iterator++ = '2';
        auto secondIterator = rb.begin();
        ++secondIterator;
        EXPECT_EQ(*secondIterator, '2');
    }

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.pop();
        rb.push('a' + i);
    }

    {
        auto iterator = rb.begin();
        EXPECT_EQ(*iterator, 'a');

        *iterator = '0';
        EXPECT_EQ(*iterator, '0');

        {
            decltype(iterator)& incIter = ++iterator;
            EXPECT_EQ(incIter, iterator);
        }
        {
            const decltype(iterator)& incIter = iterator++;
            EXPECT_NE(incIter, iterator);
        }

        iterator = rb.begin();
        *iterator++ = '1';
        EXPECT_EQ(*rb.begin(), '1');

        *iterator++ = '2';
        auto secondIterator = rb.begin();
        ++secondIterator;
        EXPECT_EQ(*secondIterator, '2');

        *iterator++ = '3';
        ++secondIterator;
        EXPECT_EQ(*secondIterator, '3');

        *iterator++ = '4';
        ++secondIterator;
        EXPECT_EQ(*secondIterator, '4');
    }
}

TEST(RingBuffer, ForwardIterator)
{
    RingBuffer<char> rb(10);

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.push('a' + i);
    }

    {
        auto iterA = rb.begin();
        auto iterB = rb.begin();

        while(iterA != rb.end() && iterB != rb.end())
        {
            EXPECT_EQ(iterA, iterB);
            EXPECT_EQ(*iterA, *iterB);
            ++iterA;
            EXPECT_NE(iterA, iterB);
            ++iterB;
        }

        EXPECT_EQ(iterA, iterB);
        EXPECT_NE(iterA, rb.begin());
        EXPECT_NE(iterB, rb.begin());

        iterA = rb.begin();
        iterB = iterA++;
        EXPECT_EQ(rb.begin(), iterB);
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ('a' + i, *iterB++);
        }
    }

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.pop();
        rb.push('a' + i);
    }

    {
        auto iterA = rb.begin();
        auto iterB = rb.begin();

        while(iterA != rb.end() && iterB != rb.end())
        {
            EXPECT_EQ(iterA, iterB);
            EXPECT_EQ(*iterA, *iterB);
            ++iterA;
            EXPECT_NE(iterA, iterB);
            ++iterB;
        }

        EXPECT_EQ(iterA, iterB);
        EXPECT_NE(iterA, rb.begin());
        EXPECT_NE(iterB, rb.begin());

        iterA = rb.begin();
        iterB = iterA++;
        EXPECT_EQ(rb.begin(), iterB);
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ('a' + i, *iterB++);
        }
    }
}

TEST(RingBuffer, ConstForwardIterator)
{
    RingBuffer<char> rb(10);

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.push('a' + i);
    }

    {
        auto iterA = rb.cbegin();
        auto iterB = rb.cbegin();

        while(iterA != rb.cend() && iterB != rb.cend())
        {
            EXPECT_EQ(iterA, iterB);
            EXPECT_EQ(*iterA, *iterB);
            ++iterA;
            EXPECT_NE(iterA, iterB);
            ++iterB;
        }

        EXPECT_EQ(iterA, iterB);
        EXPECT_NE(iterA, rb.cbegin());
        EXPECT_NE(iterB, rb.cbegin());

        iterA = rb.cbegin();
        iterB = iterA++;
        EXPECT_EQ(rb.cbegin(), iterB);
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ('a' + i, *iterB++);
        }
    }

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.pop();
        rb.push('a' + i);
    }

    {
        auto iterA = rb.cbegin();
        auto iterB = rb.cbegin();

        while(iterA != rb.cend() && iterB != rb.cend())
        {
            EXPECT_EQ(iterA, iterB);
            EXPECT_EQ(*iterA, *iterB);
            ++iterA;
            EXPECT_NE(iterA, iterB);
            ++iterB;
        }

        EXPECT_EQ(iterA, iterB);
        EXPECT_NE(iterA, rb.cbegin());
        EXPECT_NE(iterB, rb.cbegin());

        iterA = rb.cbegin();
        iterB = iterA++;
        EXPECT_EQ(rb.cbegin(), iterB);
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ('a' + i, *iterB++);
        }
    }
}

TEST(RingBuffer, BidirectionalIterator)
{
    RingBuffer<char> rb(10);

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.push('a' + i);
    }
    {
        auto iterator = rb.begin();
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(*iterator++, 'a' + i);
        }
        EXPECT_EQ(iterator, rb.end());
        for(unsigned int i = 7; i-- > 0;)
        {
            EXPECT_EQ(*--iterator, 'a' + i);
        }

        iterator = rb.end();
        for(unsigned int i = 7; i-- > 0;)
        {
            EXPECT_EQ(*--iterator, 'a' + i);
        }
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(*iterator++, 'a' + i);
        }
        EXPECT_EQ(iterator, rb.end());
    }

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.pop();
        rb.push('a' + i);
    }
    {
        auto iterator = rb.begin();
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(*iterator++, 'a' + i);
        }
        EXPECT_EQ(iterator, rb.end());
        for(unsigned int i = 7; i-- > 0;)
        {
            EXPECT_EQ(*--iterator, 'a' + i);
        }

        iterator = rb.end();
        for(unsigned int i = 7; i-- > 0;)
        {
            EXPECT_EQ(*--iterator, 'a' + i);
        }
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(*iterator++, 'a' + i);
        }
        EXPECT_EQ(iterator, rb.end());
    }

    for(unsigned int i = 7; i < 10; ++i)
    {
        rb.push('a' + i);
    }
    {
        auto iterator = rb.begin();
        for(unsigned int i = 0; i < 10; ++i)
        {
            EXPECT_EQ(*iterator++, 'a' + i);
        }
        EXPECT_EQ(iterator, rb.end());
        for(unsigned int i = 10; i-- > 0;)
        {
            EXPECT_EQ(*--iterator, 'a' + i);
        }
    }

    {
        auto iterator = rb.end();
        --iterator;
        for(unsigned int i = 10; i-- > 1;)
        {
            EXPECT_EQ(*iterator--, 'a' + i);
        }
        EXPECT_EQ(*iterator, 'a');
    }
}

TEST(RingBuffer, ConstBidirectionalIterator)
{
    RingBuffer<char> rb(10);

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.push('a' + i);
    }
    {
        auto iterator = rb.cbegin();
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(*iterator++, 'a' + i);
        }
        EXPECT_EQ(iterator, rb.cend());
        for(unsigned int i = 7; i-- > 0;)
        {
            EXPECT_EQ(*--iterator, 'a' + i);
        }

        iterator = rb.cend();
        for(unsigned int i = 7; i-- > 0;)
        {
            EXPECT_EQ(*--iterator, 'a' + i);
        }
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(*iterator++, 'a' + i);
        }
        EXPECT_EQ(iterator, rb.cend());
    }

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.pop();
        rb.push('a' + i);
    }
    {
        auto iterator = rb.cbegin();
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(*iterator++, 'a' + i);
        }
        EXPECT_EQ(iterator, rb.cend());
        for(unsigned int i = 7; i-- > 0;)
        {
            EXPECT_EQ(*--iterator, 'a' + i);
        }

        iterator = rb.cend();
        for(unsigned int i = 7; i-- > 0;)
        {
            EXPECT_EQ(*--iterator, 'a' + i);
        }
        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(*iterator++, 'a' + i);
        }
        EXPECT_EQ(iterator, rb.cend());
    }

    for(unsigned int i = 7; i < 10; ++i)
    {
        rb.push('a' + i);
    }
    {
        auto iterator = rb.cbegin();
        for(unsigned int i = 0; i < 10; ++i)
        {
            EXPECT_EQ(*iterator++, 'a' + i);
        }
        EXPECT_EQ(iterator, rb.cend());
        for(unsigned int i = 10; i-- > 0;)
        {
            EXPECT_EQ(*--iterator, 'a' + i);
        }
    }

    {
        auto iterator = rb.cend();
        --iterator;
        for(unsigned int i = 10; i-- > 1;)
        {
            EXPECT_EQ(*iterator--, 'a' + i);
        }
        EXPECT_EQ(*iterator, 'a');
    }
}

TEST(RingBuffer, RandomAccessIterator)
{
    RingBuffer<char> rb(10);

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.push('a' + i);
    }

    {
        // +=
        auto iterator = rb.begin();
        EXPECT_TRUE(iterator == (iterator += 7));
        EXPECT_EQ(rb.end(), iterator);
        iterator += -7;
        EXPECT_EQ(rb.begin(), iterator);
        iterator += 4;
        EXPECT_EQ(*iterator, 'a' + 4);
    }
    {
        // +
        auto begin = rb.begin();
        auto iterator = begin + 7;
        EXPECT_EQ(rb.end(), iterator);
        iterator = iterator + -7;
        EXPECT_EQ(rb.begin(), iterator);
        iterator = 3 + iterator;
        EXPECT_EQ(*iterator, 'a' + 3);
        iterator = -2 + iterator;
        EXPECT_EQ(*iterator, 'a' + 1);
    }
    {
        // -=
        auto iterator = rb.end();
        EXPECT_TRUE(iterator == (iterator -= 2));
        EXPECT_EQ(*iterator, 'a' + 5);
        iterator -= 3;
        EXPECT_EQ(*iterator, 'a' + 2);
        iterator -= 2;
        EXPECT_EQ(iterator, rb.begin());
    }
    {
        // - between an iterator and difference_type
        auto iterator = rb.end();
        iterator = iterator - 3;
        EXPECT_EQ(*iterator, 'a' + 4);
        iterator = iterator - 2;
        EXPECT_EQ(*iterator, 'a' + 2);
        iterator = iterator - 2;
        EXPECT_EQ(iterator, rb.begin());
    }
    {
        // - between two iterators
        auto iterA = rb.end();
        auto iterB = rb.end();
        EXPECT_EQ(iterA - iterB, 0);

        iterB = rb.begin();
        EXPECT_EQ(iterA - iterB, 7);

        iterA -= 3;
        EXPECT_EQ(iterA - iterB, 4);

        iterB += 2;
        EXPECT_EQ(iterA - iterB, 2);
    }
    {
        // []
        auto iterator = rb.begin();

        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(iterator[i], 'a' + i);
        }

        iterator += 2;

        for(unsigned int i = 0; i < 5; ++i)
        {
            EXPECT_EQ(iterator[i], 'a' + i + 2);
        }
    }
    {
        // < > <= >=
        auto iterA = rb.end();
        auto iterB = rb.begin();

        EXPECT_TRUE(iterB < iterA);
        EXPECT_TRUE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        --iterA; // 6
        EXPECT_TRUE(iterB < iterA);
        EXPECT_TRUE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        iterB += 2; // 2
        iterA -= 2; // 4
        EXPECT_TRUE(iterB < iterA);
        EXPECT_TRUE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        iterB += 2; // 4
        EXPECT_FALSE(iterB < iterA);
        EXPECT_FALSE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        iterB += 2; // 6
        EXPECT_FALSE(iterB < iterA);
        EXPECT_FALSE(iterA > iterB);
        EXPECT_FALSE(iterB <= iterA);
        EXPECT_FALSE(iterA >= iterB);

        EXPECT_FALSE(iterB < iterB);
        EXPECT_FALSE(iterB > iterB);
        EXPECT_TRUE(iterB <= iterB);
        EXPECT_TRUE(iterB >= iterB);
    }

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.pop();
        rb.push('a' + i);
    }
    {
        // +=
        auto iterator = rb.begin();
        EXPECT_TRUE(iterator == (iterator += 7));
        EXPECT_EQ(rb.end(), iterator);
        iterator += -7;
        EXPECT_EQ(rb.begin(), iterator);
        iterator += 4;
        EXPECT_EQ(*iterator, 'a' + 4);
    }
    {
        // +
        auto begin = rb.begin();
        auto iterator = begin + 7;
        EXPECT_EQ(rb.end(), iterator);
        iterator = iterator + -7;
        EXPECT_EQ(rb.begin(), iterator);
        iterator = 3 + iterator;
        EXPECT_EQ(*iterator, 'a' + 3);
        iterator = -2 + iterator;
        EXPECT_EQ(*iterator, 'a' + 1);
    }
    {
        // -=
        auto iterator = rb.end();
        EXPECT_TRUE(iterator == (iterator -= 2));
        EXPECT_EQ(*iterator, 'a' + 5);
        iterator -= 3;
        EXPECT_EQ(*iterator, 'a' + 2);
        iterator -= 2;
        EXPECT_EQ(iterator, rb.begin());
    }
    {
        // - between an iterator and difference_type
        auto iterator = rb.end();
        iterator = iterator - 3;
        EXPECT_EQ(*iterator, 'a' + 4);
        iterator = iterator - 2;
        EXPECT_EQ(*iterator, 'a' + 2);
        iterator = iterator - 2;
        EXPECT_EQ(iterator, rb.begin());
    }
    {
        // - between two iterators
        auto iterA = rb.end();
        auto iterB = rb.end();
        EXPECT_EQ(iterA - iterB, 0);

        iterB = rb.begin();
        EXPECT_EQ(iterA - iterB, 7);

        iterA -= 3;
        EXPECT_EQ(iterA - iterB, 4);

        iterB += 2;
        EXPECT_EQ(iterA - iterB, 2);
    }
    {
        // []
        auto iterator = rb.begin();

        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(iterator[i], 'a' + i);
        }

        iterator += 2;

        for(unsigned int i = 0; i < 5; ++i)
        {
            EXPECT_EQ(iterator[i], 'a' + i + 2);
        }
    }
    {
        // < > <= >=
        auto iterA = rb.end();
        auto iterB = rb.begin();

        EXPECT_TRUE(iterB < iterA);
        EXPECT_TRUE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        --iterA; // 6
        EXPECT_TRUE(iterB < iterA);
        EXPECT_TRUE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        iterB += 2; // 2
        iterA -= 2; // 4
        EXPECT_TRUE(iterB < iterA);
        EXPECT_TRUE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        iterB += 2; // 4
        EXPECT_FALSE(iterB < iterA);
        EXPECT_FALSE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        iterB += 2; // 6
        EXPECT_FALSE(iterB < iterA);
        EXPECT_FALSE(iterA > iterB);
        EXPECT_FALSE(iterB <= iterA);
        EXPECT_FALSE(iterA >= iterB);

        EXPECT_FALSE(iterB < iterB);
        EXPECT_FALSE(iterB > iterB);
        EXPECT_TRUE(iterB <= iterB);
        EXPECT_TRUE(iterB >= iterB);
    }
}

TEST(RingBuffer, ConstRandomAccessIterator)
{
    RingBuffer<char> rb(10);

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.push('a' + i);
    }

    {
        // +=
        auto iterator = rb.cbegin();
        EXPECT_TRUE(iterator == (iterator += 7));
        EXPECT_EQ(rb.cend(), iterator);
        iterator += -7;
        EXPECT_EQ(rb.cbegin(), iterator);
        iterator += 4;
        EXPECT_EQ(*iterator, 'a' + 4);
    }
    {
        // +
        auto begin = rb.cbegin();
        auto iterator = begin + 7;
        EXPECT_EQ(rb.cend(), iterator);
        iterator = iterator + -7;
        EXPECT_EQ(rb.cbegin(), iterator);
        iterator = 3 + iterator;
        EXPECT_EQ(*iterator, 'a' + 3);
        iterator = -2 + iterator;
        EXPECT_EQ(*iterator, 'a' + 1);
    }
    {
        // -=
        auto iterator = rb.cend();
        EXPECT_TRUE(iterator == (iterator -= 2));
        EXPECT_EQ(*iterator, 'a' + 5);
        iterator -= 3;
        EXPECT_EQ(*iterator, 'a' + 2);
        iterator -= 2;
        EXPECT_EQ(iterator, rb.cbegin());
    }
    {
        // - between an iterator and difference_type
        auto iterator = rb.cend();
        iterator = iterator - 3;
        EXPECT_EQ(*iterator, 'a' + 4);
        iterator = iterator - 2;
        EXPECT_EQ(*iterator, 'a' + 2);
        iterator = iterator - 2;
        EXPECT_EQ(iterator, rb.cbegin());
    }
    {
        // - between two iterators
        auto iterA = rb.cend();
        auto iterB = rb.cend();
        EXPECT_EQ(iterA - iterB, 0);

        iterB = rb.cbegin();
        EXPECT_EQ(iterA - iterB, 7);

        iterA -= 3;
        EXPECT_EQ(iterA - iterB, 4);

        iterB += 2;
        EXPECT_EQ(iterA - iterB, 2);
    }
    {
        // []
        auto iterator = rb.cbegin();

        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(iterator[i], 'a' + i);
        }

        iterator += 2;

        for(unsigned int i = 0; i < 5; ++i)
        {
            EXPECT_EQ(iterator[i], 'a' + i + 2);
        }
    }
    {
        // < > <= >=
        auto iterA = rb.cend();
        auto iterB = rb.cbegin();

        EXPECT_TRUE(iterB < iterA);
        EXPECT_TRUE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        --iterA; // 6
        EXPECT_TRUE(iterB < iterA);
        EXPECT_TRUE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        iterB += 2; // 2
        iterA -= 2; // 4
        EXPECT_TRUE(iterB < iterA);
        EXPECT_TRUE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        iterB += 2; // 4
        EXPECT_FALSE(iterB < iterA);
        EXPECT_FALSE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        iterB += 2; // 6
        EXPECT_FALSE(iterB < iterA);
        EXPECT_FALSE(iterA > iterB);
        EXPECT_FALSE(iterB <= iterA);
        EXPECT_FALSE(iterA >= iterB);

        EXPECT_FALSE(iterB < iterB);
        EXPECT_FALSE(iterB > iterB);
        EXPECT_TRUE(iterB <= iterB);
        EXPECT_TRUE(iterB >= iterB);
    }

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.pop();
        rb.push('a' + i);
    }
    {
        // +=
        auto iterator = rb.cbegin();
        EXPECT_TRUE(iterator == (iterator += 7));
        EXPECT_EQ(rb.cend(), iterator);
        iterator += -7;
        EXPECT_EQ(rb.cbegin(), iterator);
        iterator += 4;
        EXPECT_EQ(*iterator, 'a' + 4);
    }
    {
        // +
        auto begin = rb.cbegin();
        auto iterator = begin + 7;
        EXPECT_EQ(rb.cend(), iterator);
        iterator = iterator + -7;
        EXPECT_EQ(rb.cbegin(), iterator);
        iterator = 3 + iterator;
        EXPECT_EQ(*iterator, 'a' + 3);
        iterator = -2 + iterator;
        EXPECT_EQ(*iterator, 'a' + 1);
    }
    {
        // -=
        auto iterator = rb.cend();
        EXPECT_TRUE(iterator == (iterator -= 2));
        EXPECT_EQ(*iterator, 'a' + 5);
        iterator -= 3;
        EXPECT_EQ(*iterator, 'a' + 2);
        iterator -= 2;
        EXPECT_EQ(iterator, rb.cbegin());
    }
    {
        // - between an iterator and difference_type
        auto iterator = rb.cend();
        iterator = iterator - 3;
        EXPECT_EQ(*iterator, 'a' + 4);
        iterator = iterator - 2;
        EXPECT_EQ(*iterator, 'a' + 2);
        iterator = iterator - 2;
        EXPECT_EQ(iterator, rb.cbegin());
    }
    {
        // - between two iterators
        auto iterA = rb.cend();
        auto iterB = rb.cend();
        EXPECT_EQ(iterA - iterB, 0);

        iterB = rb.cbegin();
        EXPECT_EQ(iterA - iterB, 7);

        iterA -= 3;
        EXPECT_EQ(iterA - iterB, 4);

        iterB += 2;
        EXPECT_EQ(iterA - iterB, 2);
    }
    {
        // []
        auto iterator = rb.cbegin();

        for(unsigned int i = 0; i < 7; ++i)
        {
            EXPECT_EQ(iterator[i], 'a' + i);
        }

        iterator += 2;

        for(unsigned int i = 0; i < 5; ++i)
        {
            EXPECT_EQ(iterator[i], 'a' + i + 2);
        }
    }
    {
        // < > <= >=
        auto iterA = rb.cend();
        auto iterB = rb.cbegin();

        EXPECT_TRUE(iterB < iterA);
        EXPECT_TRUE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        --iterA; // 6
        EXPECT_TRUE(iterB < iterA);
        EXPECT_TRUE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        iterB += 2; // 2
        iterA -= 2; // 4
        EXPECT_TRUE(iterB < iterA);
        EXPECT_TRUE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        iterB += 2; // 4
        EXPECT_FALSE(iterB < iterA);
        EXPECT_FALSE(iterA > iterB);
        EXPECT_TRUE(iterB <= iterA);
        EXPECT_TRUE(iterA >= iterB);

        iterB += 2; // 6
        EXPECT_FALSE(iterB < iterA);
        EXPECT_FALSE(iterA > iterB);
        EXPECT_FALSE(iterB <= iterA);
        EXPECT_FALSE(iterA >= iterB);

        EXPECT_FALSE(iterB < iterB);
        EXPECT_FALSE(iterB > iterB);
        EXPECT_TRUE(iterB <= iterB);
        EXPECT_TRUE(iterB >= iterB);
    }
}

TEST(RingBuffer, IteratorMisc)
{
    RingBuffer<char> rb(10);

    // empty, begin and end should be the same
    EXPECT_EQ(rb.begin(), rb.end());

    rb.push('a');
    EXPECT_NE(rb.begin(), rb.end());

    rb.pop();

    for(unsigned int i = 0; i < 10; ++i)
    {
        rb.push('a' + i);
    }

    {
        unsigned int i = 0;
        for(auto& reference : rb)
        {
            EXPECT_EQ('a' + i, reference);
            reference = 'A' + i;
            ++i;
        }

        i = 0;
        for(const auto& constRef : rb)
        {
            EXPECT_EQ('A' + i, constRef);
            ++i;
        }
    }

    char a = rb.pop();
    char b = rb.pop();

    rb.push('A' + 10);
    rb.push('A' + 11);

    EXPECT_EQ(a, 'A');
    EXPECT_EQ(b, 'B');

    for(unsigned int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(rb.at(i), 'A' + i + 2);
    }
}

TEST(RingBuffer, ChangeCapacity)
{
    RingBuffer<char> rb(10);

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.push('a' + i);
        rb.pop();
    }
    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.push('a' + i);
    }

    rb.changeCapacity(7);
    EXPECT_EQ(7, rb.getCapacity());
    EXPECT_EQ(7, rb.getSize());

    for(unsigned int i = 0; i < 7; ++i)
    {
        EXPECT_EQ('a' + i, rb.at(i));
    }

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.pop();
        rb.push('a' + i);
    }

    rb.changeCapacity(4);
    EXPECT_EQ(4, rb.getCapacity());
    EXPECT_EQ(4, rb.getSize());

    for(unsigned int i = 0; i < 4; ++i)
    {
        EXPECT_EQ('a' + i, rb.at(i));
    }

    rb.changeCapacity(10);
    EXPECT_EQ(10, rb.getCapacity());
    EXPECT_EQ(4, rb.getSize());

    for(unsigned int i = 0; i < 4; ++i)
    {
        EXPECT_EQ('a' + i, rb.at(i));
    }

    rb.changeCapacity(0);
    EXPECT_TRUE(rb.empty());
    EXPECT_EQ(0, rb.getSize());

    rb.changeCapacity(4);
    EXPECT_TRUE(rb.empty());
    EXPECT_EQ(0, rb.getSize());

    for(unsigned int i = 0; i < 4; ++i)
    {
        rb.push('a' + i);
    }
    EXPECT_FALSE(rb.empty());
    EXPECT_EQ(4, rb.getSize());
}

TEST(RingBuffer, ChangeSize)
{
    RingBuffer<char> rb(10);

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.push('a' + i);
    }

    EXPECT_EQ(7, rb.getSize());

    rb.changeSize(4);
    EXPECT_EQ(4, rb.getSize());
    for(unsigned int i = 0; i < 4; ++i)
    {
        EXPECT_EQ('a' + i, rb.at(i));
    }

    rb.changeSize(7, 'z');
    EXPECT_EQ(7, rb.getSize());
    for(unsigned int i = 0; i < 7; ++i)
    {
        if(i < 4)
        {
            EXPECT_EQ('a' + i, rb.at(i));
        }
        else
        {
            EXPECT_EQ('z', rb.at(i));
        }
    }

    for(unsigned int i = 0; i < 7; ++i)
    {
        rb.pop();
        rb.push('a' + i);
    }

    for(unsigned int i = 0; i < 7; ++i)
    {
        EXPECT_EQ('a' + i, rb.at(i));
    }

    EXPECT_EQ(7, rb.getSize());
    rb.changeSize(5);
    EXPECT_EQ(5, rb.getSize());

    for(unsigned int i = 0; i < 5; ++i)
    {
        EXPECT_EQ('a' + i, rb.at(i));
    }

    rb.changeSize(10, 'z');
    EXPECT_EQ(10, rb.getSize());

    for(unsigned int i = 0; i < 10; ++i)
    {
        if(i < 5)
        {
            EXPECT_EQ('a' + i, rb.at(i));
        }
        else
        {
            EXPECT_EQ('z', rb.at(i));
        }
    }
}

