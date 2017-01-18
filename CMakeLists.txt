cmake_minimum_required(VERSION 3.0)

project(RingBuffer)

set(RING_BUFFER_HEADERS
    src/RB/RingBuffer.hpp
    src/RB/RingBuffer.inl
)

set(UNIT_TEST_SOURCES
    src/UnitTest/main.cpp
    src/UnitTest/TestRingBuffer.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -D NDEBUG")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug', none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

find_package(GTest QUIET)

if(GTEST_FOUND)
    add_executable(UnitTest ${UNIT_TEST_SOURCES})

    target_include_directories(UnitTest
        PUBLIC src
    )

    target_link_libraries(UnitTest
        PUBLIC ${GTEST_BOTH_LIBRARIES}
    )
endif()

