#include "instance.hpp"

#include <iostream>

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            SPDLOG_TRACE("Validation layer: {}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            SPDLOG_INFO("Validation layer: {}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            SPDLOG_WARN("Validation layer: {}", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            SPDLOG_ERROR("Validation layer: {}", pCallbackData->pMessage);
            break;
        default:
            SPDLOG_ERROR("Validation layer: {}", pCallbackData->pMessage);
            break;
    }
    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void gpu::instance::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debug_callback;
}

void gpu::instance::setup_debug_messenger(VkInstance instance, VkDebugUtilsMessengerEXT& debugMessenger)
{
    if (!enable_validation_layers) { return; }

    SPDLOG_INFO("Setting up debug messenger...");
    VkDebugUtilsMessengerCreateInfoEXT create_info ;
    populate_debug_messenger_create_info(create_info);

    if (CreateDebugUtilsMessengerEXT(instance, &create_info, nullptr, &debugMessenger) != VK_SUCCESS) {
        SPDLOG_ERROR("Failed to set up debug messenger");
        throw std::runtime_error("Failed to set up debug messenger");
    }
    SPDLOG_INFO("Set up debug messenger");
}



/*
 * Fails if init_glfw is not called
 */
VkInstance gpu::instance::create_instance(const std::string& application_name)
{
    SPDLOG_INFO("Creating VkApplicationInfo...");
    VkApplicationInfo app_info {
        .pApplicationName = application_name.c_str(), 
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0
    };
    SPDLOG_INFO("Created VkApplicationInfo");

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> enabled_extension_names;

    for(uint32_t i = 0; i < glfwExtensionCount; i++) {
        enabled_extension_names.emplace_back(glfwExtensions[i]);
    }
    enabled_extension_names.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    enabled_extension_names.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    enabled_extension_names.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    uint32_t enabled_extension_count = static_cast<uint32_t>(enabled_extension_names.size());

    if (enable_validation_layers && !check_layer_support(validation_layers)) {
        SPDLOG_ERROR("Validation layers requested, but not available!");
        throw std::runtime_error("Validation layers not found");
    }

    uint32_t enabled_layer_count = 0;
    const char* const* enabled_layer_names = nullptr;
    VkDebugUtilsMessengerCreateInfoEXT debug_create_info {};
    const void * pNext = nullptr;
    if (enable_validation_layers) {
        enabled_layer_count = static_cast<uint32_t>(validation_layers.size());
        enabled_layer_names = validation_layers.data(); 
        populate_debug_messenger_create_info(debug_create_info);
        pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_create_info;
    }

    gpu::extension::check_extension_support(required_extensions);

    VkInstanceCreateInfo create_info {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = pNext,
        .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = enabled_layer_count,
        .ppEnabledLayerNames = enabled_layer_names,
        .enabledExtensionCount = enabled_extension_count,
        .ppEnabledExtensionNames = enabled_extension_names.data()
    };

    SPDLOG_INFO("Creating VkInstance...");
    VkInstance instance;
    VkResult result = vkCreateInstance(&create_info, nullptr, &instance);
    if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS) {
        SPDLOG_ERROR("Failed to create VkInstance");
        throw std::runtime_error("Failed to create VkInstance");
    } 
    SPDLOG_INFO("Created VkInstance");

    return instance;
}

void gpu::instance::cleanup(VkInstance instance) {
    // vkDestroyInstance(instance, nullptr);

    SPDLOG_INFO("Destroyed VkInstance");
}


