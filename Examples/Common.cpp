//
// Created by Gaetz on 16/04/2025.
//

#include "Common.hpp"

int CommonInit(Context* context, SDL_WindowFlags windowFlags)
{
    context->device = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
        false,
        NULL);

    if (context->device == NULL)
    {
        SDL_Log("GPUCreateDevice failed");
        return -1;
    }

    context->window = SDL_CreateWindow(context->exampleName, 1280, 720, windowFlags);
    if (context->window == NULL)
    {
        SDL_Log("CreateWindow failed: %s", SDL_GetError());
        return -1;
    }

    if (!SDL_ClaimWindowForGPUDevice(context->device, context->window))
    {
        SDL_Log("GPUClaimWindow failed");
        return -1;
    }

    return 0;
}