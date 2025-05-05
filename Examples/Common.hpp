//
// Created by Gaetz on 16/04/2025.
//

#ifndef COMMON_HPP
#define COMMON_HPP

#include <SDL3/SDL.h>

struct Context
{
    const char* example_name;
    const char* base_path;
    SDL_Window* window;
    SDL_GPUDevice* device;
    bool left_pressed;
    bool right_pressed;
    bool down_pressed;
    bool up_pressed;
    float delta_time;
};

int CommonInit(Context* context, SDL_WindowFlags window_flags);

void InitializeAssetLoader(Context* context);

SDL_GPUShader* LoadShader(SDL_GPUDevice* device, const char* base_path, const char* shader_filename,
                          Uint32 sampler_count, Uint32 uniform_buffer_count, Uint32 storage_buffer_count, Uint32 storage_texture_count);

void CommonQuit(Context* context);

#endif //COMMON_HPP
