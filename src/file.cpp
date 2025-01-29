#include "file.hpp"

namespace vkBLAS {
namespace utils {

std::vector<char> read_binary_file(const std::string& filename)
{
    SPDLOG_INFO("Initiating reading of file: {}", filename);
    // 
    std::ifstream file(filename, std::ios::ate | std::ios::binary); 

    if (!file.is_open()) {
        SPDLOG_ERROR("Failed to open file: {}", filename);
        throw std::runtime_error("Failed to open file: " + filename);
    }

    size_t file_size = file.tellg();
    SPDLOG_INFO("Size of file \"{}\" is: {}", filename, file_size);

    std::vector<char> buffer(file_size);

    file.seekg(0);
    file.read(buffer.data(), file_size);

    SPDLOG_INFO("Read file \"{}\" successfully", filename);

    file.close();

    return buffer;
}

std::string read_text_file(const std::string& filename)
{
    SPDLOG_INFO("Initiating reading of file: {}", filename);
    std::ifstream file(filename, std::ios::in | std::ios::binary); 

    if (!file.is_open()) {
        SPDLOG_ERROR("Failed to open file: {}", filename);
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string content((std::istreambuf_iterator<char>(file)), 
                      std::istreambuf_iterator<char>());
    file.close();

    SPDLOG_INFO("Read file \"{}\" successfully", filename);
    SPDLOG_INFO("Contents of file \"{}\"", content.c_str());
    return content;
}

void write_file(const std::string& filename, const std::string& contents)
{
    SPDLOG_INFO("Initiating writing to file: {}", filename);
    std::ofstream file(filename, std::ios::out | std::ios::binary);

    if (!file.is_open()) {
        SPDLOG_ERROR("Failed to open file: {}", filename);
        throw std::runtime_error("Failed to open file: " + filename);
    }

    file << contents.c_str();

    file.close();

    SPDLOG_INFO("Wrote to file \"{}\" successfully", filename);
}

void write_file(const std::string& filename, const std::vector<uint32_t>& contents)
{
    SPDLOG_INFO("Initiating writing to file: {}", filename);
    std::ofstream file(filename, std::ios::out | std::ios::binary);

    if (!file.is_open()) {
        SPDLOG_ERROR("Failed to open file: {}", filename);
        throw std::runtime_error("Failed to open file: " + filename);
    }

    for (auto& word : contents) {
        file.write(reinterpret_cast<const char*>(&word), sizeof(uint32_t));
    }

    file.close();

    SPDLOG_INFO("Wrote to file \"{}\" successfully", filename);
}

} // namespace utils
} // namespace vkBLAS
