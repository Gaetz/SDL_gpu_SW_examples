//
// Created by gaetz on 05/05/25.
//

#include "Buffer.hpp"

void Buffer::Create()
{
    const SDL_GPUBufferCreateInfo buffer_create_info = {
        .usage = usage_flags_,
        .size = size_
    };
    buffer_ = SDL_CreateGPUBuffer(device_, &buffer_create_info);
}

void Buffer::AddVertexBinding(const u32 offset)
{
    vertex_bindings_.push_back({.buffer = buffer_, .offset = offset});
}

void Buffer::Release() const
{
    SDL_ReleaseGPUBuffer(device_, buffer_);
}
