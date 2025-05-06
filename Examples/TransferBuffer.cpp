//
// Created by gaetz on 05/05/25.
//

#include "TransferBuffer.hpp"
#include "Buffer.hpp"

TransferBuffer::TransferBuffer(SDL_GPUDevice* device, const u32 size, SDL_GPUTransferBufferUsage usage)
    : device_(device), size_(size), usage_(usage)
{
    const SDL_GPUTransferBufferCreateInfo create_info = {
        .usage = usage,
        .size = size
    };
    transfer_buffer_ = SDL_CreateGPUTransferBuffer(device, &create_info);
}

TransferBuffer::~TransferBuffer()
{
    Release();
}

void TransferBuffer::Unmap() const
{
    SDL_UnmapGPUTransferBuffer(device_, transfer_buffer_);
}

void TransferBuffer::AddSourceUpload(const u32 offset)
{
    sources_.push_back({.transfer_buffer = transfer_buffer_, .offset = offset});
}

void TransferBuffer::AddDestinationUpload(const Buffer& buffer, const u32 offset, const u32 size)
{
    destinations_.push_back({.buffer = buffer.GetBufferPtr(), .offset = offset, .size = size});
}

void TransferBuffer::Upload(const bool cycle) const
{
    SDL_GPUCommandBuffer* cmdBuf = SDL_AcquireGPUCommandBuffer(device_);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdBuf);
    for (int i = 0; i < sources_.size(); ++i)
    {
        SDL_UploadToGPUBuffer(copyPass, &sources_[i], &destinations_[i], cycle);
    }
    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(cmdBuf);
}

void TransferBuffer::Release() const
{
    SDL_ReleaseGPUTransferBuffer(device_, transfer_buffer_);
}
