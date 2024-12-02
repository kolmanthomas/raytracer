#include "layer.hpp"

bool check_layer_support(std::vector<const char*> layers)
{
    SPDLOG_INFO("Checking layer support...");

    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    SPDLOG_INFO("Number of layers present: {}", layer_count);

    std::vector<VkLayerProperties> available_layer_properties { layer_count };
    vkEnumerateInstanceLayerProperties(&layer_count, available_layer_properties.data());

    std::vector<const char*> available_layers { layer_count };
    for (int i = 0; i < layer_count; i++) {
        const char* layer_name = available_layer_properties[i].layerName;
        SPDLOG_INFO("Available layer: {}", layer_name);
        available_layers[i] = layer_name;
    }

    if (vector_intersection(layers, available_layers).size() != layers.size()) {
        SPDLOG_ERROR("Missing required extensions");
        return false;
    }
    SPDLOG_INFO("All required extensions are supported");

    return true;
}


