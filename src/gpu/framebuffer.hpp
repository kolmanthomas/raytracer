#pragma once

#include "src/gpu/gpu.hpp"

namespace gpu {


std::vector<VkFramebuffer> create_framebuffer(const std::vector<VkImageView>& swap_chain_image_views, VkDevice device, VkRenderPass render_pass);

} //namespace gpu
