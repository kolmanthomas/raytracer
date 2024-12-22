#include "physical_device.hpp"

namespace gpu {

// @exception Immediately throw an exception if any of this fails
LIGHTLY_API std::vector<VkQueueFamilyProperties> find_queue_families(VkPhysicalDevice device, const std::vector<VkQueueFlags>& required_queue_flags)
{
    SPDLOG_INFO("Fetching queue families...");

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr); // Does not fail

    std::vector<VkQueueFamilyProperties> queue_families { queue_family_count };
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data()); // Does not fail

    return queue_families;
}

LIGHTLY_API SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    SPDLOG_INFO("Querying swap chain support...");
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

    if (format_count != 0) {
        details.formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
    }

    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);

    if (present_mode_count != 0) {
        details.present_modes.resize(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.present_modes.data());
    }

    return details;
}
static std::vector<VkExtensionProperties> get_device_extensions(VkPhysicalDevice device)
{
    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions { extension_count };
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

    return available_extensions;
}

/*
static bool check_device_extension_support(VkPhysicalDevice device, std::vector<const char*> required_device_extensions)
{
    std::set<std::string> required_extensions { required_device_extensions.begin(), required_device_extensions.end() };

    for (const auto& extension : available_extensions) {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}
*/

// @exception Immediately throw an exception if any of this fails
static LightlyDevice get_device_info(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceFeatures device_features;

    vkGetPhysicalDeviceProperties(device, &device_properties); // Does not fail
    vkGetPhysicalDeviceFeatures(device, &device_features); // Does not fail

    SPDLOG_INFO("==========================================================");
    SPDLOG_INFO("Fetching device info of \"{}\"", device_properties.deviceName);
    SPDLOG_INFO("");
    SPDLOG_INFO("Device type is: {}", string_VkPhysicalDeviceType(device_properties.deviceType));    

    auto queue_families = find_queue_families(device, { VK_QUEUE_GRAPHICS_BIT }); // May fail, allow to propogate
    SPDLOG_INFO("Found {} queue families", queue_families.size());

    int i = 0;
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;
    for (const auto& queue_family : queue_families) {
        SPDLOG_INFO("");
        SPDLOG_INFO("Queue family: {}", i);
        SPDLOG_INFO("--- Queue flags: {}", string_VkQueueFlags(queue_family.queueFlags));  
        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphics_family = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) {
            SPDLOG_INFO("--- Queue family supports presentation");
            present_family = i;
        }

        i++;
    }

    SPDLOG_INFO("");
    auto extensions = get_device_extensions(device);
    SPDLOG_INFO("Device extensions supported:");
    for (const auto& e : extensions) {
        SPDLOG_INFO("--- {}", e.extensionName);
    }

    SPDLOG_INFO("Support geometry shader: {}", device_features.geometryShader ? "Yes" : "No"); 

    SPDLOG_INFO("==========================================================");
    
    LightlyDevice lightly_device = {
        .device = device,
        .device_properties = device_properties,
        .device_features = device_features,
        .graphics_family = graphics_family,
        .present_family = present_family,
        .extensions = extensions,
    };

    return lightly_device;
}

// @note 
LIGHTLY_API static bool is_device_suitable(const LightlyDevice& lightly_device, VkSurfaceKHR surface, const std::vector<const char*>& required_device_extensions)
{

    // Check device extension support
    std::set<std::string> required_extensions { required_device_extensions.begin(), required_device_extensions.end() };

    for (const auto& extension : lightly_device.extensions) {
        required_extensions.erase(extension.extensionName);
    }

    if (!required_extensions.empty()) {
        SPDLOG_INFO("Device does not support required extensions");
        return 0;
    }
    SPDLOG_INFO("Device \"{}\" supports all required extensions", lightly_device.device_properties.deviceName);

    SwapChainSupportDetails swap_chain_support = query_swap_chain_support(lightly_device.device, surface);
    bool swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();

    if (!swap_chain_adequate) {
        SPDLOG_INFO("Device does not support swap chain");
        return 0;
    }

    /*
    if (!lightly_device.device_features.geometryShader) {
        SPDLOG_INFO("Device does not support geometry shaders");
        return 0;
    }
    */

    // Queue family indices are supported
    if (!lightly_device.is_complete()) {
        return 0;
    }

    SPDLOG_INFO("Finishing ranking device suitability of \"{}\"", lightly_device.device_properties.deviceName);

    return true;
}


LIGHTLY_API static uint32_t rate_device_suitability(const LightlyDevice& lightly_device, VkSurfaceKHR surface, const std::vector<const char*>& required_device_extensions)
{
    SPDLOG_INFO("Beginning ranking device suitability of \"{}\"", lightly_device.device_properties.deviceName);
    uint32_t score = 0;

    // First, check the NEED TO HAVE stuff
    if (!is_device_suitable(lightly_device, surface, required_device_extensions)) {
        SPDLOG_INFO("Device \"{}\" was not found suitable", lightly_device.device_properties.deviceName);
        return 0;
    }

    // Then, check the "nice to have" stuff
    if (lightly_device.device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    // Don't need this for now
    // score += device_properties.limits.maxImageDimension2D;

    SPDLOG_INFO("Device suitability score of \"{}\" is {}", lightly_device.device_properties.deviceName, score);

    return score;
}

// Returns vector of physical devices.
// @exception
static std::vector<VkPhysicalDevice> get_physical_devices(VkInstance& instance)
{
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr); // May fail, but we check result value second time
    SPDLOG_INFO("Found {} physical devices with Vulkan support", device_count);

    // Not strictly an error, but for application purposes, we need a GPU
    if (device_count == 0) {
        SPDLOG_ERROR("No GPU with Vulkan support found!");
        throw std::runtime_error("No GPU with Vulkan support found!");
    }
    
    std::vector<VkPhysicalDevice> physical_devices(device_count);
    if (vkEnumeratePhysicalDevices(instance, &device_count, physical_devices.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to enumerate physical devices"); 
    }

    return physical_devices;
}

// Directly picks the physical device with the requested name.
//
// @exception Throws if no GPU with Vulkan support is found.
VkPhysicalDevice pick_physical_device(VkInstance& instance, const std::string& requested_device_name) 
{
    SPDLOG_INFO("Requested physical device: {}", requested_device_name);

    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    std::vector<VkPhysicalDevice> devices = get_physical_devices(instance);

    for (const auto& device : devices) {
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(device, &device_properties);

        if (std::strcmp(device_properties.deviceName, requested_device_name.c_str()) == 0) {
            SPDLOG_INFO("Found device: {}", requested_device_name);
            return device;
        }
    }

    SPDLOG_ERROR("No physical device with the requested name found!");
    throw std::runtime_error("No suitable GPU found!");
}

// Picks the best physical device based on a scoring system.
//
// @exception Throws if no GPU with Vulkan support is found.
//
// @note Uses a scoring system w/ requirements to pick the best GPU.
LIGHTLY_API LightlyDevice pick_physical_device(VkInstance& instance, VkSurfaceKHR surface, const std::vector<const char*>& required_device_extensions) 
{
    LightlyDevice lightly_device;
    std::vector<VkPhysicalDevice> physical_devices = get_physical_devices(instance);
    std::vector<LightlyDevice> lightly_devices { physical_devices.size() };

    uint32_t highest_score = 0;
    for (const auto& pd : physical_devices) {
        LightlyDevice pd_info = get_device_info(pd, surface);

        uint32_t score = rate_device_suitability(pd_info, surface, required_device_extensions); 
        if (score >= highest_score) {
            lightly_device = pd_info;
            highest_score = score;
        }
    }

    if (highest_score == 0) {
        SPDLOG_ERROR("No suitable GPU found!");
        throw std::runtime_error("No suitable GPU found!");
    }

    SPDLOG_INFO("Selected device: {}", lightly_device.device_properties.deviceName);

    return lightly_device;
}

} // namespace gpu
