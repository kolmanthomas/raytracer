#pragma once

// #include "glslang/Public/ShaderLang.h"
#include "glslang/Include/glslang_c_interface.h"
#include <glslang/Public/resource_limits_c.h>
#include <spdlog/spdlog.h>

#include <string>
#include <cstdlib>

/*
struct SPIRVBinary {
    uint32_t* words; // SPIR-V words
    size_t size; // number of words in SPIR-V binary
};

struct ShaderSource {
    std::string name;
    std::string source;
};

inline SPIRVBinary compile_shader_to_spirv(const ShaderSource& shader_source, glslang_stage_t stage)
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
*/

