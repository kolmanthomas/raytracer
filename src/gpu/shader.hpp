#pragma once

#include "src/gpu/gpu.hpp"
#include "src/gpu/logical_device.hpp"

#include <vector> 

namespace gpu {

VkShaderModule create_shader_module(VkDevice device, const std::vector<char>& code);

}
