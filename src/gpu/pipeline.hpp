#pragma once

#include "src/gpu/gpu.hpp"
#include "src/gpu/shader.hpp"

#include <tuple>

namespace gpu {

std::tuple<VkPipelineLayout, VkPipeline> create_pipeline(
    VkDevice device,
    VkShaderModule vert_shader_module,
    VkShaderModule frag_shader_module,
    VkRenderPass renderpass
//    VkDescriptorSetLayout descriptor_set_layout
);

} // namespace gpu
