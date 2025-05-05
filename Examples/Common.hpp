//
// Created by Gaetz on 16/04/2025.
//

#ifndef COMMON_HPP
#define COMMON_HPP

#include <SDL3/SDL.h>

struct Context
{
    const char* example_name_{};
    const char* base_path_{};
    SDL_Window* window_ = nullptr;
    SDL_GPUDevice* device_ = nullptr;
    bool left_pressed_{};
    bool right_pressed_{};
    bool down_pressed_{};
    bool up_pressed_{};
    float delta_time_{};
};

int CommonInit(Context& context, SDL_WindowFlags window_flags);

void InitializeAssetLoader(Context& context);

void CommonQuit(const Context& context);

#endif //COMMON_HPP
