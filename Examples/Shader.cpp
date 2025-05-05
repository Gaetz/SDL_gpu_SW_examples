//
// Created by gaetz on 05/05/25.
//

#include "Shader.hpp"

#include "SDL3/SDL_log.h"

Shader::Shader(
    SDL_GPUDevice* device,
    const char* base_path,
    const char* shader_filename,
    const Uint32 sampler_count,
    const Uint32 uniform_buffer_count,
    const Uint32 storage_buffer_count,
    const Uint32 storage_texture_count
)
{
    device_ = device;
    base_path_ = base_path;
    shader_filename_ = shader_filename;

    // Auto-detect the shader stage from the file name for convenience
    SDL_GPUShaderStage stage;
    if (SDL_strstr(shader_filename, ".vert"))
    {
        stage = SDL_GPU_SHADERSTAGE_VERTEX;
    }
    else if (SDL_strstr(shader_filename, ".frag"))
    {
        stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    }
    else
    {
        SDL_Log("Invalid shader stage!");
        return;
    }

    char fullPath[256];
    const SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
    SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
    const char* entrypoint;

    if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV)
    {
        SDL_snprintf(fullPath, sizeof(fullPath), "%sContent/Shaders/Compiled/SPIRV/%s.spv", base_path, shader_filename);
        format = SDL_GPU_SHADERFORMAT_SPIRV;
        entrypoint = "main";
    }
    else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL)
    {
        SDL_snprintf(fullPath, sizeof(fullPath), "%sContent/Shaders/Compiled/MSL/%s.msl", base_path, shader_filename);
        format = SDL_GPU_SHADERFORMAT_MSL;
        entrypoint = "main0";
    }
    else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL)
    {
        SDL_snprintf(fullPath, sizeof(fullPath), "%sContent/Shaders/Compiled/DXIL/%s.dxil", base_path, shader_filename);
        format = SDL_GPU_SHADERFORMAT_DXIL;
        entrypoint = "main";
    }
    else
    {
        SDL_Log("%s", "Unrecognized backend shader format!");
        return;
    }

    size_t codeSize;
    void* code = SDL_LoadFile(fullPath, &codeSize);
    if (code == nullptr)
    {
        SDL_Log("Failed to load shader from disk! %s", fullPath);
        return;
    }

    const SDL_GPUShaderCreateInfo shaderInfo = {
        .code_size = codeSize,
        .code = static_cast<const Uint8*>(code),
        .entrypoint = entrypoint,
        .format = format,
        .stage = stage,
        .num_samplers = sampler_count,
        .num_storage_textures = storage_texture_count,
        .num_storage_buffers = storage_buffer_count,
        .num_uniform_buffers = uniform_buffer_count
    };
    shader_ = SDL_CreateGPUShader(device, &shaderInfo);
    if (shader_ == nullptr)
    {
        switch (stage)
        {
            case SDL_GPU_SHADERSTAGE_VERTEX:
                SDL_Log("Failed to create vertex shader!");
                break;
            case SDL_GPU_SHADERSTAGE_FRAGMENT:
                SDL_Log("Failed to create fragment shader!");
                break;
            default:
                SDL_Log("Failed to create shader!");
                break;
        }
        SDL_free(code);
        return;
    }

    SDL_free(code);
}

Shader::~Shader()
{
    if (shader_ != nullptr)
    {
        SDL_ReleaseGPUShader(device_, shader_);
        shader_ = nullptr;
    }
}
