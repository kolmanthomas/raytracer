#pragma once

#include "src/gpu/gpu.hpp"
#include "src/gpu/physical_device.hpp"

namespace gpu {

/*
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};
*/
inline VkFormat swap_chain_image_format;
inline VkExtent2D swap_chain_extent;

VkSwapchainKHR create_swap_chain(VkPhysicalDevice physical_device, VkDevice device, VkSurfaceKHR surface, GLFWwindow* window, uint32_t width, uint32_t height);
std::vector<VkImage> get_swap_chain_images(VkDevice device, VkSwapchainKHR swap_chain);

}

