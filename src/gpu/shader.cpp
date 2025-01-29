#include "src/gpu/shader.hpp"

namespace gpu {

VkShaderModule create_shader_module(VkDevice device, const SPIRVBinary& code)
{
    /*
    std::vector<const uint32_t> code_new;
    for (int i = 0; i < code.size(); i++) {
        code_new.push_back(reinterpret_cast<const uint32_t>(code[i]));
    }
    */

    VkShaderModuleCreateInfo create_info {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = code.size * sizeof(uint32_t),
        .pCode = code.words
    };

    VkShaderModule shader_module;
    if (vkCreateShaderModule(device, &create_info, nullptr, &shader_module) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module");
    }
    SPDLOG_INFO("Shader module created");

    return shader_module;
}

SPIRVBinary compile_shader_to_spirv(const ShaderSource& shader_source, glslang_stage_t stage)
{
    const char* cstr_shader_source = shader_source.source.c_str();   
    const glslang_input_t input = {
        .language = GLSLANG_SOURCE_GLSL,
        .stage = stage,
        .client = GLSLANG_CLIENT_VULKAN,
        .client_version = GLSLANG_TARGET_VULKAN_1_2,
        .target_language = GLSLANG_TARGET_SPV,
        .target_language_version = GLSLANG_TARGET_SPV_1_0,
        .code = cstr_shader_source,
        .default_version = 450,
        .default_profile = GLSLANG_NO_PROFILE,
        .force_default_version_and_profile = false,
        .forward_compatible = false,
        .messages = GLSLANG_MSG_DEFAULT_BIT,
        .resource = glslang_default_resource(),
    };
    glslang_initialize_process();
    glslang_shader_t* shader = glslang_shader_create(&input);

    SPIRVBinary bin {
        .words = {},
        .size = 0
    };

    if (!glslang_shader_preprocess(shader, &input))	{
        SPDLOG_ERROR("GLSL preprocessing failed {}\nInfo:{}, Debug:{}\nCode:{}", 
                     shader_source.name,
                     glslang_shader_get_info_log(shader),
                     glslang_shader_get_info_debug_log(shader),
                     input.code);
        glslang_shader_delete(shader);
        throw std::runtime_error("GLSL preprocessing failed");
    }
    SPDLOG_INFO("{}: GLSL preprocessing succeeded", shader_source.name);

    if (!glslang_shader_parse(shader, &input)) {
        SPDLOG_ERROR("GLSL parsing failed {}\nInfo:{}\n Debug:{}\nCode:{}", 
                     shader_source.name,
                     glslang_shader_get_info_log(shader),
                     glslang_shader_get_info_debug_log(shader),
                     glslang_shader_get_preprocessed_code(shader));
        glslang_shader_delete(shader);
        throw std::runtime_error("GLSL parsing failed");
    }
    SPDLOG_INFO("{}: GLSL parsing succeeded", shader_source.name);

    glslang_program_t* program = glslang_program_create();
    glslang_program_add_shader(program, shader);

    if (!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT)) {
        SPDLOG_ERROR("{}: GLSL linking failed\nInfo:{}\nDebug:{}", 
                     shader_source.name,
                     glslang_program_get_info_log(program),
                     glslang_program_get_info_debug_log(program));
        glslang_program_delete(program);
        glslang_shader_delete(shader);
        throw std::runtime_error("GLSL linking failed");
    }
    SPDLOG_INFO("{}: GLSL linking succeeded", shader_source.name);

    glslang_program_SPIRV_generate(program, stage);

    bin.size = glslang_program_SPIRV_get_size(program);
    bin.words = static_cast<uint32_t*>(malloc(bin.size * sizeof(uint32_t)));
    glslang_program_SPIRV_get(program, bin.words);

    const char* spirv_messages = glslang_program_SPIRV_get_messages(program);
    if (spirv_messages) {
        SPDLOG_INFO("({}) {}\b", shader_source.name , spirv_messages);
    }

    glslang_program_delete(program);
    glslang_shader_delete(shader);

    return bin;
}



} // namespace gpu
