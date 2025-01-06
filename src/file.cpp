#include "file.hpp"

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
    std::ifstream file(filename, std::ios::ate | std::ios::binary); 

    if (!file.is_open()) {
        SPDLOG_ERROR("Failed to open file: {}", filename);
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string content((std::istreambuf_iterator<char>(file)), 
                      std::istreambuf_iterator<char>());
    file.close();
    return content;
}
