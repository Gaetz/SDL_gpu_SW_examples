//
// Created by Gaetz on 16/04/2025.
//

#include "Common.hpp"

int CommonInit(Context& context, SDL_WindowFlags window_flags)
{
    context.device_ = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
        true,
        nullptr);

    if (context.device_ == nullptr)
    {
        SDL_Log("GPUCreateDevice failed");
        return -1;
    }

    context.window_ = SDL_CreateWindow(context.example_name_, 1280, 720, window_flags);
    if (context.window_ == nullptr)
    {
        SDL_Log("CreateWindow failed: %s", SDL_GetError());
        return -1;
    }

    if (!SDL_ClaimWindowForGPUDevice(context.device_, context.window_))
    {
        SDL_Log("GPUClaimWindow failed");
        return -1;
    }

    return 0;
}

void InitializeAssetLoader(Context& context)
{
    context.base_path_ = SDL_GetBasePath();
}

void CommonQuit(const Context& context)
{
    SDL_ReleaseWindowFromGPUDevice(context.device_, context.window_);
    SDL_DestroyWindow(context.window_);
    SDL_DestroyGPUDevice(context.device_);
}
