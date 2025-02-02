#pragma once

#include "src/gpu/gpu.hpp"
#include "src/gpu/physical_device.hpp"
#include "src/gpu/shader.hpp"

namespace gpu {

VkCommandPool create_command_pool(gpu::LightlyDevice phys_device_struct, VkDevice device);
std::vector<VkCommandBuffer> create_command_buffers(VkDevice device, VkCommandPool command_pool); 
void record_command_buffer(
    VkCommandBuffer command_buffer, 
    VkRenderPass render_pass, 
    const std::vector<VkFramebuffer>& swap_chain_framebuffers, 
    uint32_t image_index, 
    VkPipeline graphics_pipeline,
    VkBuffer vertex_buffer,
    VkBuffer index_buffer,
    std::vector<Vertex> vertices,
    std::vector<uint16_t> indices);

} // namespace gpu
