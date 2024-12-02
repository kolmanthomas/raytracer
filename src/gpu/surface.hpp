#pragma once

#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

namespace gpu {
namespace surface {

VkSurfaceKHR create_surface(VkInstance instance, GLFWwindow* window);

}
}
