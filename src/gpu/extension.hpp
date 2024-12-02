#pragma once

#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h" 
#include "vulkan/vulkan.h"

#include "src/handy.hpp"

#include <set>
#include <vector>

namespace gpu {
namespace extension {

bool check_extension_support(const std::vector<const char*>& extensions);
} // namespace extension
} // namespace gpu

