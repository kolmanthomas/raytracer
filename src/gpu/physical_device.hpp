#pragma once

#include "src/gpu/gpu.hpp"

#include "src/handy.hpp"

#include <cstring>
#include <set>
#include <vector>

namespace gpu {
namespace physical_device {

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;

    bool is_complete() const
    {
        return graphics_family.has_value() && present_family.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};


struct LightlyDevice {
    VkPhysicalDevice device;
    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceFeatures device_features;

    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;

    std::vector<VkExtensionProperties> extensions;

    bool is_complete() const
    {
        return graphics_family.has_value() && present_family.has_value();
    }
};

LIGHTLY_API std::vector<VkQueueFamilyProperties> find_queue_families(VkPhysicalDevice device, const std::vector<VkQueueFlags>& required_queue_flags);
LIGHTLY_API VkPhysicalDevice pick_physical_device(VkInstance& instance, const std::string& requested_device_name); 
LIGHTLY_API LightlyDevice pick_physical_device(VkInstance& instance, VkSurfaceKHR surface, const std::vector<const char*>& required_device_extensions);

}
}
