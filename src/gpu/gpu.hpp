#pragma once

#define GLFW_INCLUDE_NONE
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

// Vulkan must be included first.
#include "vulkan/vulkan.h"

#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include "vulkan/vk_enum_string_helper.h"
#include "vulkan/vulkan_beta.h"

#define LIGHTLY_API [[nodiscard]]

#ifdef NDEBUG
    constexpr bool enable_validation_layers = false;
#else 
    constexpr bool enable_validation_layers = true;
#endif

const std::vector<const char*> validation_layers= {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> required_extensions {
    VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
};



