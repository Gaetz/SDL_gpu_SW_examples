//
// Created by gaetz on 05/05/25.
//

#ifndef SHADER_HPP
#define SHADER_HPP
#include "SDL3/SDL_gpu.h"


class Shader {
public:
    Shader() = default;

    Shader(
        SDL_GPUDevice* device,
        const char* base_path,
        const char* shader_filename,
        Uint32 sampler_count,
        Uint32 uniform_buffer_count,
        Uint32 storage_buffer_count,
        Uint32 storage_texture_count
    );

    ~Shader();

    [[nodiscard]] SDL_GPUShader* GetShaderPtr() const { return shader_; }
    [[nodiscard]] bool IsValid() const { return shader_ != nullptr; }

private:
    SDL_GPUShader* shader_ = nullptr;
    SDL_GPUDevice* device_ = nullptr;
    const char* base_path_ = nullptr;
    const char* shader_filename_ = nullptr;
};



#endif //SHADER_HPP
