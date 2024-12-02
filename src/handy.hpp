#pragma once

#include <spdlog/spdlog.h>

#include <algorithm>
#include <vector>

std::vector<std::string> vector_intersection(std::vector<std::string> v1, std::vector<std::string> v2);
std::vector<const char*> vector_intersection(std::vector<const char*> v1, std::vector<const char*> v2);

template <typename T>
void log_vector(const std::vector<T>& v, const std::string& message)
{
    std::string full_message = message + ": {}";
    for (const auto& i : v) {
        SPDLOG_INFO(full_message, i);
    }
}

inline const char * const bool_to_str(bool b)
{
  return b ? "True" : "False";
}
