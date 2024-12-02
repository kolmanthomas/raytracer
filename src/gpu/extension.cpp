#include "extension.hpp"

//
//
// @note Vector type is const char* because Vulkan enums convert to const char*
bool gpu::extension::check_extension_support(const std::vector<const char*>& extensions)
{
    SPDLOG_INFO("Checking extension support...");

    uint32_t extension_count;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    SPDLOG_INFO("Number of extensions present: {}", extension_count);

    std::vector<VkExtensionProperties> available_extension_properties{ extension_count };
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extension_properties.data());

    std::vector<const char *> available_extensions {extension_count};
    for (int i = 0; i < extension_count; i++) {
        const char * extension_name = available_extension_properties[i].extensionName;
        SPDLOG_INFO("Available extension: {}", extension_name);
        available_extensions[i] = extension_name;
    }

    for (const auto& extension : extensions) {
        SPDLOG_INFO("Requested extension: {}", extension);
    }

    if (vector_intersection(extensions, available_extensions).size() != extensions.size()) {
        SPDLOG_ERROR("Missing required extensions");
        return false;
    }
    SPDLOG_INFO("All required extensions are supported");

    return true;
}

