#include "src/gpu/command_pool.hpp"
#include "src/gpu/swap_chain.hpp"

namespace gpu {

VkCommandPool create_command_pool(gpu::LightlyDevice phys_device_struct, VkDevice device)
{
    VkCommandPoolCreateInfo pool_info {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = phys_device_struct.graphics_family.value() 
    };

    VkCommandPool command_pool;
    if (vkCreateCommandPool(device, &pool_info, nullptr, &command_pool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool!");
    }

    return command_pool;
}

std::vector<VkCommandBuffer> create_command_buffers(VkDevice device, VkCommandPool command_pool) 
{
    std::vector<VkCommandBuffer> command_buffers(gpu::global::max_frames_in_flight);

    VkCommandBufferAllocateInfo alloc_info {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = static_cast<uint32_t>(gpu::global::max_frames_in_flight)
    };
 
    if (vkAllocateCommandBuffers(device, &alloc_info, command_buffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers!");
    }

    return command_buffers;
}

void record_command_buffer(
    VkCommandBuffer command_buffer, 
    VkRenderPass render_pass, 
    const std::vector<VkFramebuffer>& swap_chain_framebuffers, 
    uint32_t image_index, 
    VkPipeline graphics_pipeline,
    VkBuffer vertex_buffer,
    VkBuffer index_buffer,
    std::vector<Vertex> vertices,
    std::vector<uint16_t> indices)
{
    VkCommandBufferBeginInfo begin_info {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0,
        .pInheritanceInfo = nullptr,
    };
    
    if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    /*
    VkRect2D render_area {
        .offset = {0, 0},
        .extent = gpu::swap_chain_extent
    };
    */

    VkRenderPassBeginInfo render_pass_info {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = render_pass,
        .framebuffer = swap_chain_framebuffers[image_index],
        .clearValueCount = 1,
        .pClearValues = &clear_color,
    };
    render_pass_info.renderArea.offset = {0, 0};
    render_pass_info.renderArea.extent = gpu::swap_chain_extent;

    vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);

        VkBuffer vertex_buffers[] = { vertex_buffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);

        VkViewport viewport {
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(gpu::swap_chain_extent.width),
            .height = static_cast<float>(gpu::swap_chain_extent.height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };
        vkCmdSetViewport(command_buffer, 0, 1, &viewport);

        VkRect2D scissor {
            .offset = {0, 0},
            .extent = gpu::swap_chain_extent
        };
        vkCmdSetScissor(command_buffer, 0, 1, &scissor);

        vkCmdBindIndexBuffer(command_buffer, index_buffer, 0, VK_INDEX_TYPE_UINT16);
        vkCmdDrawIndexed(command_buffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(command_buffer);

    if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }
}
} // namespace gpu
