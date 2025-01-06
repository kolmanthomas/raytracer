#pragma once

#include "src/gpu/gpu.hpp"
#include "src/gpu/shader.hpp"

namespace gpu {

struct Buffer {
    VkBuffer data;
    VkDeviceMemory memory;
};

struct UniformBuffer {
    std::vector<VkBuffer> uniform_buffers;
    std::vector<VkDeviceMemory> uniform_buffers_memory;
    std::vector<void*> uniform_buffers_mapped;
};

Buffer create_vertex_buffer(VkPhysicalDevice physical_device, VkDevice device, std::vector<Vertex> vertices, VkCommandPool command_pool, VkQueue graphics_queue);


Buffer create_index_buffer(VkPhysicalDevice physical_device, VkDevice device, std::vector<uint16_t> indices, VkCommandPool command_pool, VkQueue graphics_queue);


UniformBuffer create_uniform_buffer(VkPhysicalDevice physical_device, VkDevice device) ;

}
