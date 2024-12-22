#include "src/gpu/framebuffer.hpp"
#include "src/gpu/swap_chain.hpp"

namespace gpu {

std::vector<VkFramebuffer> create_framebuffer(const std::vector<VkImageView>& swap_chain_image_views, VkDevice device, VkRenderPass render_pass)
{
    std::vector<VkFramebuffer> swap_chain_framebuffers(swap_chain_image_views.size()); 

    for (size_t i = 0; i < swap_chain_image_views.size(); i++) {
        VkImageView attachments[] = {
            swap_chain_image_views[i]
        };

        VkFramebufferCreateInfo framebuffer_info {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = render_pass,
            .attachmentCount = 1,
            .pAttachments = attachments,
            .width = gpu::swap_chain_extent.width,
            .height = gpu::swap_chain_extent.height,
            .layers = 1
        };

        if (vkCreateFramebuffer(device, &framebuffer_info, nullptr, &swap_chain_framebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create framebuffer!");
        }
    }

    return swap_chain_framebuffers;
}

} //namespace gpu
