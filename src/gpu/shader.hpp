#pragma once

#include "src/gpu/gpu.hpp"
#include "src/gpu/logical_device.hpp"
#include "glslang/Include/glslang_c_interface.h"
#include <glslang/Public/resource_limits_c.h>

#include <Eigen/Dense>
#include <glm/glm.hpp>

#include <array>
#include <vector> 

struct SPIRVBinary {
    uint32_t* words; // SPIR-V words
    size_t size; // number of words in SPIR-V binary
};

struct ShaderSource {
    std::string name;
    std::string source;
};


struct Vertex {
public:
    Eigen::Vector2f m_pos; 
    Eigen::Vector3f m_color; 

    static inline VkVertexInputBindingDescription get_binding_description()
    {
        VkVertexInputBindingDescription binding_description {
            .binding = 0, 
            .stride = sizeof(Vertex),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
        };
        return binding_description;
    }

    static std::array<VkVertexInputAttributeDescription, 2> get_attribute_descriptions()
    {
        std::array<VkVertexInputAttributeDescription, 2> attribute_descriptions {
            VkVertexInputAttributeDescription {
                .location = 0,
                .binding = 0,
                .format = VK_FORMAT_R32G32_SFLOAT,
                .offset = offsetof(Vertex, m_pos)
            },
            VkVertexInputAttributeDescription {
                .location = 1,
                .binding = 0,
                .format = VK_FORMAT_R32G32B32_SFLOAT,
                .offset = offsetof(Vertex, m_color)
            }
        };
        return attribute_descriptions;
    }
};

struct UniformBufferObject {
    /*
    Eigen::Matrix4f model; 
    Eigen::Matrix4f view; 
    Eigen::Matrix4f proj; 
    */
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

namespace gpu {

VkShaderModule create_shader_module(VkDevice device, const SPIRVBinary& code);

SPIRVBinary compile_shader_to_spirv(const ShaderSource& shader_source, glslang_stage_t stage);


}
