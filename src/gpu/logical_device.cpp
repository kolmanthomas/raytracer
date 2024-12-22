#include "src/gpu/logical_device.hpp"

namespace gpu {

VkQueue get_graphics_queue(const gpu::LightlyDevice& physical_device, VkDevice logical_device)
{
    VkQueue queue;
    vkGetDeviceQueue(logical_device, physical_device.graphics_family.value(), 0, &queue);
    return queue;
}

VkDevice create_logical_device(const gpu::LightlyDevice& physical_device, const std::vector<const char*>& required_device_extensions)
{
    SPDLOG_INFO("Creating logical device...");
    VkDevice device;

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::set<uint32_t> unique_queue_families = { physical_device.graphics_family.value(), physical_device.present_family.value() };

    for (uint32_t queue_family : unique_queue_families) {
        VkDeviceQueueCreateInfo queue_create_info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = queue_family,
            .queueCount = 1,
            .pQueuePriorities = new float(1.0f),
        };

        queue_create_infos.push_back(queue_create_info);
    }

    VkPhysicalDeviceFeatures device_features = {};

    VkDeviceCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()),
        .pQueueCreateInfos = queue_create_infos.data(),
        .enabledExtensionCount = static_cast<uint32_t>(required_device_extensions.size()),
        .ppEnabledExtensionNames = required_device_extensions.data(),
        .pEnabledFeatures = &device_features,
    };

    if (enable_validation_layers) {
        create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        create_info.ppEnabledLayerNames = validation_layers.data();
    } else {
        create_info.enabledLayerCount = 0;
    }

    VkResult result = vkCreateDevice(physical_device.device, &create_info, nullptr, &device);
    if (result != VK_SUCCESS) {
        SPDLOG_ERROR("Failed to create logical device!");
        throw std::runtime_error("Failed to create logical device!");
    }

    SPDLOG_INFO("Logical device created successfully");
    return device;
}

} // namespace gpu
