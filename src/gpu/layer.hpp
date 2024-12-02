#pragma once

#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h" 
#include "vulkan/vulkan.h"

#include "src/handy.hpp"

bool check_layer_support(std::vector<const char*> layers);
