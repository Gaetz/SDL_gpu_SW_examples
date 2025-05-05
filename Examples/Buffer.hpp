//
// Created by gaetz on 05/05/25.
//

#ifndef BUFFER_HPP
#define BUFFER_HPP
#include "Defines.hpp"
#include "SDL3/SDL_gpu.h"

class Buffer
{
public:
    Buffer() = default;
    ~Buffer();

    [[nodiscard]] SDL_GPUBuffer* GetBufferPtr() const { return buffer_; }
    [[nodiscard]] const Vector<SDL_GPUBufferBinding>& GetVertexBindings() const { return vertex_bindings_; }
    void SetDevice(SDL_GPUDevice* device) { device_ = device; }
    void SetSize(const u32 size) { size_ = size; }
    void SetUsageFlags(const SDL_GPUBufferUsageFlags usage_flags) { usage_flags_ = usage_flags; }

    void Create();
    void AddVertexBinding(const u32 offset);
    void Release() const;

    SDL_GPUBuffer* buffer_ = nullptr;

private:
    SDL_GPUDevice* device_ = nullptr;
    u32 size_ = 0;
    SDL_GPUBufferUsageFlags usage_flags_ = 0;
    Vector<SDL_GPUBufferBinding> vertex_bindings_{};
};


#endif //BUFFER_HPP
