#include "src/gpu/shader.hpp"

namespace gpu {

VkShaderModule create_shader_module(VkDevice device, const std::vector<char>& code)
{
    VkShaderModuleCreateInfo create_info {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = code.size(),
        .pCode = reinterpret_cast<const uint32_t*>(code.data()),
    };

    VkShaderModule shader_module;
    if (vkCreateShaderModule(device, &create_info, nullptr, &shader_module) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module");
    }

    return shader_module;
}


} // namespace gpu

/*
VkVertexInputBindingDescription Vertex::get_binding_description()
{
    VkVertexInputBindingDescription binding_description {
        .binding = 0, 
        .stride = sizeof(Vertex),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };
    return binding_description;
}

std::array<VkVertexInputAttributeDescription, 2> Vertex::get_attribute_descriptions()
{
    std::array<VkVertexInputAttributeDescription, 2> attribute_descriptions {
        VkVertexInputAttributeDescription {
            .location = 0,
            .binding = 0,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = offsetof(Vertex, pos)
        },
        VkVertexInputAttributeDescription {
            .location = 1,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = offsetof(Vertex, color)
        }
    };
    return attribute_descriptions;
}

*/
