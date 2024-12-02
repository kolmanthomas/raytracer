#pragma once

#include "src/gpu/gpu.hpp"

#include "src/gpu/extension.hpp"
#include "src/gpu/layer.hpp"

#include <vector>
#include <string>

namespace gpu {
namespace instance {

void setup_debug_messenger(VkInstance instance, VkDebugUtilsMessengerEXT& debugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator); 
VkInstance create_instance(const std::string& application_name);
void cleanup(VkInstance instance);

}
} // namespace gpu
