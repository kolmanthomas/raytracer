#include "src/gpu/surface.hpp"

VkSurfaceKHR gpu::surface::create_surface(VkInstance instance, GLFWwindow* window)
{
    SPDLOG_INFO("Creating window surface...");

    VkSurfaceKHR surface;

    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        SPDLOG_ERROR("Failed to create window surface!");
        throw std::runtime_error("Failed to create window surface!");
    }

    SPDLOG_INFO("Created window surface");

    return surface;
}
