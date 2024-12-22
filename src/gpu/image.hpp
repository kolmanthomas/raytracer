#pragma once

#include "src/gpu/gpu.hpp"

#include "src/gpu/swap_chain.hpp"

namespace gpu {

std::vector<VkImageView> create_image_views(const std::vector<VkImage>& swap_chain_images, VkDevice device);

} //namespace gpu
