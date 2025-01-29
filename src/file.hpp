#pragma once

#include <spdlog/spdlog.h>

#include <vector>
#include <fstream>

namespace vkBLAS {
namespace utils {

template<typename T>
struct File {
    std::string name;
    std::vector<T> contents;  
};

std::vector<char> read_binary_file(const std::string& filename);
std::string read_text_file(const std::string& filename);
void write_file(const std::string& filename, const std::vector<uint32_t>& contents);

} // namespace utils
} // namespace vkBLAS
