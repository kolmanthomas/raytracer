#pragma once


#include <cstddef>
#include <deque>
#include <optional>

template <typename T, size_t N>
class CircularBuffer {
public:
    constexpr CircularBuffer() = default;

    constexpr void push(T value)
    {
        if (buffer.size() == N)
        {
            buffer.pop_back();
        }
        buffer.push_front(value); 
    }

    constexpr std::optional<T> moving_average()
    {
        T sum = 0;
        for (auto it = buffer.begin(); it != buffer.end(); ++it)
        {
            // Check for overflow 
            if ((*it > 0 && sum > std::numeric_limits<T>::max() - *it) || (*it < 0 && sum < std::numeric_limits<T>::min() - *it)) {
                return std::nullopt;
            } 
            sum += *it;
        }

        // One more check to make sure that buffer.size is not 0
        if (buffer.size() == 0)
        {
            return std::nullopt;
        }
        return sum / buffer.size();
    }

    // No pop method
private:
    std::deque<T> buffer;
};
