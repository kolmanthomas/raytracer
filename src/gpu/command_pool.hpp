#pragma once

#include "src/gpu/gpu.hpp"
#include "src/gpu/physical_device.hpp"

namespace gpu {

VkCommandPool create_command_pool(gpu::LightlyDevice phys_device_struct, VkDevice device);
VkCommandBuffer create_command_buffer(VkDevice device, VkCommandPool command_pool);
void record_command_buffer(VkCommandBuffer command_buffer, VkRenderPass render_pass, const std::vector<VkFramebuffer>& swap_chain_framebuffers, uint32_t image_index, VkPipeline graphics_pipeline);

} // namespace gpu
