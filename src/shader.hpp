#pragma once

// #include "glslang/Public/ShaderLang.h"
#include "glslang/Include/glslang_c_interface.h"
#include <glslang/Public/resource_limits_c.h>
#include <spdlog/spdlog.h>

#include <string>
#include <cstdlib>

struct SPIRVBinary {
    std::vector<uint32_t> words; // SPIR-V words
    size_t size; // number of words in SPIR-V binary
};

struct ShaderSource {
    std::string name;
    std::string source;
};

SPIRVBinary compile_shader_to_spirv(const ShaderSource& shader_source, glslang_stage_t stage)
{
    const char* cstr_shader_source = shader_source.source.c_str();   
    const glslang_input_t input = {
        .language = GLSLANG_SOURCE_GLSL,
        .stage = stage,
        .client = GLSLANG_CLIENT_VULKAN,
        .client_version = GLSLANG_TARGET_VULKAN_1_2,
        .target_language = GLSLANG_TARGET_SPV,
        .target_language_version = GLSLANG_TARGET_SPV_1_6,
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

    glslang_program_SPIRV_generate(program, stage);

    bin.size = glslang_program_SPIRV_get_size(program);
    bin.words.resize(bin.size);
    glslang_program_SPIRV_get(program, bin.words.data());

    const char* spirv_messages = glslang_program_SPIRV_get_messages(program);
    if (spirv_messages) {
        SPDLOG_INFO("({}) {}\b", shader_source.name , spirv_messages);
    }

    glslang_program_delete(program);
    glslang_shader_delete(shader);

    return bin;



    /*
    glslang::InitializeProcess();

    glslang::TShader vert_shader(EShLangVertex); 

    const char* shader_source_cstr[1] = { shader_source.c_str() };
    vert_shader.setStrings(shader_source_cstr, 1);

    // Use appropriate Vulkan version
    glslang::EShTargetClientVersion targetApiVersion = glslang::EShTargetVulkan_1_3;
    vert_shader.setEnvClient(glslang::EShClientVulkan, targetApiVersion);

    glslang::EShTargetLanguageVersion spirvVersion = glslang::EShTargetSpv_1_3;
    vert_shader.setEnvTarget(glslang::EshTargetSpv, spirvVersion);

    vert_shader.setEntryPoint("main"); // We can specify a different entry point
    
    // Parse and verify shaders
    if (!vert_shader.parse(&DefaultTBuiltInResource, 450, false, EShMsgDefault)) {
        cerr << "Vertex shader compilation failed:\n" << vertShader->getInfoLog() << endl;
        return 1;
    }
    */

}

