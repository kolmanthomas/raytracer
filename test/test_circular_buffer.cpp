#include <gtest/gtest.h>

#include "src/circular_buffer.hpp"

TEST(CircularBuffer, BasicAssertions) {
    CircularBuffer<int, 5> buffer;
    buffer.push(1);
    buffer.push(1);
    buffer.push(2);
    buffer.push(3);
    buffer.push(4);
    buffer.push(5);
    ASSERT_EQ(buffer.moving_average(), 16);
}

TEST(CircularBuffer, FillWithTime) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    auto s = duration_cast<std::chrono::milliseconds>(end - begin).count();

    std::cout << s << std::endl;

    /*
    CircularBuffer<int, 5> buffer;
    buffer.push(1);
    buffer.push(1);
    buffer.push(2);
    buffer.push(3);
    buffer.push(4);
    buffer.push(5);
    buffer.push(6);
    ASSERT_EQ(buffer.moving_average(), 15);
    */
}
