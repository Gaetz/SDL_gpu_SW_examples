//
// Created by Gaetz on 16/04/2025.
//

#include "Demo001_Basics_BasicTriangle.hpp"

#include <memory>

#include "Data.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.hpp"
#include "TransferBuffer.hpp"

// Example with an instanced vertex buffer, if we donc want to use a big uniform
//https://github.com/TheSpydog/SDL_gpu_examples/blob/93d94af8a9008309e99ca1b6b415476685c66501/Examples/PullSpriteBatch.c

int Demo001_Basics_BasicTriangle::Init(const Context& context)
{
    // Create the shaders
    Shader vertex_shader{
        context.device_, context.base_path_, "001_triangle.vert", 0, 1, 0, 0
    };
    if (!vertex_shader.IsValid()) return -1;


    Shader fragment_shader{
        context.device_, context.base_path_, "001_triangle.frag", 0, 0, 0, 0
    };
    if (!fragment_shader.IsValid()) return -1;

    // Create the pipelines
    pipeline_.SetDevice(context.device_);
    pipeline_.SetVertexShader(vertex_shader);
    pipeline_.SetFragmentShader(fragment_shader);
    const Vector<SDL_GPUVertexBufferDescription> vertex_buffer_descriptions = {
        {0, sizeof(PositionColorVertex), SDL_GPU_VERTEXINPUTRATE_VERTEX, 0}
    };
    const Vector<SDL_GPUVertexAttribute> vertex_attributes = {
        {0, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, 0},
        {1, 0, SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM, sizeof(float) * 3}
    };
    pipeline_.SetVertexInputState(vertex_buffer_descriptions, vertex_attributes);
    pipeline_.SetPrimitiveType(PrimitiveType::TriangleList);
    pipeline_.SetRasterizerState(FillMode::Fill);
    const Vector<SDL_GPUColorTargetDescription> color_target_descriptions = {
        {SDL_GetGPUSwapchainTextureFormat(context.device_, context.window_)}
    };
    pipeline_.SetTargetInfo(color_target_descriptions);
    pipeline_.Create();

    // Setup the vertex buffer
    vertex_buffer_.SetDevice(context.device_);
    vertex_buffer_.SetSize(sizeof(PositionColorVertex) * 3);
    vertex_buffer_.SetUsageFlags(SDL_GPU_BUFFERUSAGE_VERTEX);
    vertex_buffer_.Create();

    // To get data into the vertex buffer, we have to use a transfer buffer
    TransferBuffer transfer_buffer{context.device_, sizeof(PositionColorVertex) * 3, SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD};

    // Map the transfer buffer and fill it with data (data is bound to the transfer buffer)
    const auto transferData = transfer_buffer.Map<PositionColorVertex>();
    transferData[0] = (PositionColorVertex){-0.5, -0.5, 0, 255, 0, 0, 255};
    transferData[1] = (PositionColorVertex){0, 0.5, 0, 0, 0, 0, 255};
    transferData[2] = (PositionColorVertex){0.5, -0.5, 0, 0, 255, 0, 255};
    transfer_buffer.Unmap();

    // Upload the transfer data to the vertex buffer
    transfer_buffer.AddSourceUpload(0);
    transfer_buffer.AddDestinationUpload(vertex_buffer_, 0, sizeof(PositionColorVertex) * 3);
    transfer_buffer.Upload();

    // Default MVP matrix
    mvp_ = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f) *
        glm::lookAt(glm::vec3(0, 0, -2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)) *
        glm::mat4(1.0f);
    return 0;
}

void Demo001_Basics_BasicTriangle::Update(const Context& context)
{
}

void Demo001_Basics_BasicTriangle::Draw(const Context& context)
{
    SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(context.device_);
    if (command_buffer == nullptr)
    {
        SDL_Log("AcquireGPUCommandBuffer failed: %s", SDL_GetError());
        return;
    }

    SDL_GPUTexture* swapchain_texture;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, context.window_, &swapchain_texture, nullptr,
                                               nullptr))
    {
        SDL_Log("WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
        return;
    }

    if (swapchain_texture != nullptr)
    {
        // Clear info
        SDL_GPUColorTargetInfo color_target_info = {nullptr};
        color_target_info.texture = swapchain_texture;
        color_target_info.clear_color = SDL_FColor{0.0f, 0.0f, 0.2f, 1.0f};
        color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
        color_target_info.store_op = SDL_GPU_STOREOP_STORE;

        // Render pass
        SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(command_buffer, &color_target_info, 1, nullptr);
        pipeline_.Bind(render_pass);
        SDL_PushGPUVertexUniformData(command_buffer, 0, &mvp_, sizeof(mvp_));
        vertex_buffer_.AddVertexBinding(0);
        vertex_buffer_.BindVertexBuffer(render_pass, 0);
        SDL_DrawGPUPrimitives(render_pass, 3, 1, 0, 0);
        SDL_EndGPURenderPass(render_pass);
    }

    SDL_SubmitGPUCommandBuffer(command_buffer);
}

void Demo001_Basics_BasicTriangle::Quit(const Context& context)
{
    pipeline_.Release();
}
