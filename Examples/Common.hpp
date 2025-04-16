//
// Created by Gaetz on 16/04/2025.
//

#ifndef COMMON_HPP
#define COMMON_HPP

#include <SDL3/SDL.h>

typedef struct Context
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
} Context;

int CommonInit(Context* context, SDL_WindowFlags windowFlags);




#endif //COMMON_HPP
