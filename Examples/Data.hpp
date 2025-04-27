//
// Created by gaetz on 16/04/2025.
//

#ifndef DATA_HPP
#define DATA_HPP

#include <SDL3/SDL_stdinc.h>

struct PositionVertex
{
    float x, y, z;
};

struct PositionColorVertex
{
    float x, y, z;
    Uint8 r, g, b, a;
};

struct PositionTextureVertex
{
    float x, y, z;
    float u, v;
};

#endif //DATA_HPP
