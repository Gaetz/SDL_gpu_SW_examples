//
// Created by Gaetz on 16/04/2025.
//

#ifndef COMMON_HPP
#define COMMON_HPP

#include <SDL3/SDL.h>

struct Context
{
    const char* exampleName;
    const char* basePath;
    SDL_Window* window;
    SDL_GPUDevice* device;
    bool leftPressed;
    bool rightPressed;
    bool downPressed;
    bool upPressed;
    float deltaTime;
};

int CommonInit(Context* context, SDL_WindowFlags windowFlags);

void InitializeAssetLoader(Context* context);

SDL_GPUShader* LoadShader(SDL_GPUDevice* device, const char* basePath, const char* shaderFilename,
                          Uint32 samplerCount, Uint32 uniformBufferCount, Uint32 storageBufferCount, Uint32 storageTextureCount);

void CommonQuit(Context* context);

#endif //COMMON_HPP
