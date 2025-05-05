//
// Created by gaetz on 05/05/25.
//

#ifndef TRANSFERBUFFER_HPP
#define TRANSFERBUFFER_HPP
#include "Defines.hpp"
#include "SDL3/SDL_gpu.h"

enum class TransferBufferUsage
{
    Upload = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
    Download = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD
};

class TransferBuffer
{
public:
    TransferBuffer(SDL_GPUDevice* device, u32 size, SDL_GPUTransferBufferUsage usage);
    ~TransferBuffer();

    SDL_GPUTransferBuffer* GetBufferPtr() const { return transfer_buffer_; }

    template <class T>
    T* Map(const bool cycle = false)
    {
        return static_cast<T*>(SDL_MapGPUTransferBuffer(device_, transfer_buffer_, cycle));
    }

    void Unmap() const;

    void AddSourceUpload(u32 offset);
    void AddDestinationUpload(const class Buffer& buffer, u32 offset, u32 size);
    void Upload(bool cycle = false) const;

    void Release() const;

private:
    SDL_GPUDevice* device_ = nullptr;
    SDL_GPUTransferBuffer* transfer_buffer_ = nullptr;
    u32 size_ = 0;
    //TransferBufferUsage usage_ = TransferBufferUsage::Upload;
    SDL_GPUTransferBufferUsage usage_ = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    Vector<SDL_GPUTransferBufferLocation> sources_;
    Vector<SDL_GPUBufferRegion> destinations_;
};


#endif //TRANSFERBUFFER_HPP
