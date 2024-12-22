#pragma once

#include <spdlog/spdlog.h>

#include <vector>
#include <fstream>

std::vector<char> read_binary_file(const std::string& filename);
