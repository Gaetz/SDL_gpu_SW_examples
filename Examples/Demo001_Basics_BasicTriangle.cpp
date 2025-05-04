//
// Created by Gaetz on 16/04/2025.
//

#include "Demo001_Basics_BasicTriangle.hpp"
#include "Data.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Example with an instanced vertex buffer, if we donc want to use a big uniform
//https://github.com/TheSpydog/SDL_gpu_examples/blob/93d94af8a9008309e99ca1b6b415476685c66501/Examples/PullSpriteBatch.c

int Demo001_Basics_BasicTriangle::Init(Context* context) {
    // Init SDL, window and device
    int result = CommonInit(context, 0);
    if (result < 0) { return result; }

    // Create the shaders
    SDL_GPUShader* vertexShader = LoadShader(context->device, context->basePath,
                                             "001_triangle.vert", 0, 1, 0, 0);
    if (vertexShader == nullptr) {
        SDL_Log("Failed to create vertex shader!");
        return -1;
    }

    SDL_GPUShader* fragmentShader = LoadShader(context->device, context->basePath,
                                               "001_triangle.frag", 0, 0, 0, 0);
    if (fragmentShader == nullptr) {
        SDL_Log("Failed to create fragment shader!");
        return -1;
    }

    // Create the pipelines
    SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {
            .vertex_shader = vertexShader,
            .fragment_shader = fragmentShader,
            .vertex_input_state = SDL_GPUVertexInputState {
                    .vertex_buffer_descriptions = new SDL_GPUVertexBufferDescription[1] {{
                                                                                                 .slot = 0,
                                                                                                 .pitch = sizeof(PositionColorVertex),
                                                                                                 .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                                                                                                 .instance_step_rate = 0,
                                                                                         }},
                    .num_vertex_buffers = 1,
                    .vertex_attributes = new SDL_GPUVertexAttribute[2] {{
                                                                                .location = 0,
                                                                                .buffer_slot = 0,
                                                                                .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                                                                                .offset = 0
                                                                        },
                                                                        {
                                                                                .location = 1,
                                                                                .buffer_slot = 0,
                                                                                .format = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM,
                                                                                .offset = sizeof(float) * 3
                                                                        }},
                    .num_vertex_attributes = 2,
            },
            .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,

            .target_info = {
                    .color_target_descriptions = new SDL_GPUColorTargetDescription[1] {
                            {
                                    .format = SDL_GetGPUSwapchainTextureFormat(context->device, context->window)
                            }
                    },
                    .num_color_targets = 1,
            },

    };

    pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
    pipeline = SDL_CreateGPUGraphicsPipeline(context->device, &pipelineCreateInfo);
    if (pipeline == nullptr) {
        SDL_Log("Failed to create fill pipeline!");
        return -1;
    }

    // Clean up shader resources
    SDL_ReleaseGPUShader(context->device, vertexShader);
    SDL_ReleaseGPUShader(context->device, fragmentShader);

    // Create the vertex buffer
    SDL_GPUBufferCreateInfo vertexBufferCreateInfo = {
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size = sizeof(PositionColorVertex) * 3
    };
    vertexBuffer = SDL_CreateGPUBuffer(context->device, &vertexBufferCreateInfo);

    // To get data into the vertex buffer, we have to use a transfer buffer
    constexpr SDL_GPUTransferBufferCreateInfo transferBufferCreateInfo = {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = sizeof(PositionColorVertex) * 3
    };
    SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(context->device, &transferBufferCreateInfo);

    // Map the transfer buffer and fill it with data (data is bound to the transfer buffer)
    auto transferData = static_cast<PositionColorVertex*>(SDL_MapGPUTransferBuffer(context->device, transferBuffer, false));
    transferData[0] = (PositionColorVertex) {  -0.5,  -0.5, 0, 255,   0,   0, 255 };
    transferData[1] = (PositionColorVertex) {   0.5,  -0.5, 0, 0, 255,   0, 255 };
    transferData[2] = (PositionColorVertex) {     0,   0.5, 0, 0, 0,   0, 255 };
    SDL_UnmapGPUTransferBuffer(context->device, transferBuffer);

    // Upload the transfer data to the vertex buffer
    SDL_GPUTransferBufferLocation transferBufferLocation = {
        .transfer_buffer = transferBuffer,
        .offset = 0
    };
    SDL_GPUBufferRegion vertexBufferRegion = {
        .buffer = vertexBuffer,
        .offset = 0,
        .size = sizeof(PositionColorVertex) * 3
    };

    // Upload the transfer data to the vertex buffer
    SDL_GPUCommandBuffer* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(context->device);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);

    SDL_UploadToGPUBuffer(copyPass, &transferBufferLocation, &vertexBufferRegion, false);

    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(uploadCmdBuf);
    SDL_ReleaseGPUTransferBuffer(context->device, transferBuffer);

    // Default MVP matrix
    mvp = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f) *
         glm::lookAt(glm::vec3(0, 0, -2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)) *
        glm::mat4(1.0f);
    return 0;
}

int Demo001_Basics_BasicTriangle::Update(Context* context)
{
    return 0;
}

int Demo001_Basics_BasicTriangle::Draw(Context* context) {
    SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(context->device);
    if (commandBuffer == nullptr) {
        SDL_Log("AcquireGPUCommandBuffer failed: %s", SDL_GetError());
        return -1;
    }

    SDL_GPUTexture* swapchainTexture;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, context->window, &swapchainTexture, nullptr, nullptr)) {
        SDL_Log("WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
        return -1;
    }

    if (swapchainTexture != nullptr) {
        SDL_GPUColorTargetInfo colorTargetInfo = { nullptr };
        colorTargetInfo.texture = swapchainTexture;
        colorTargetInfo.clear_color = SDL_FColor { 0.0f, 0.0f, 0.2f, 1.0f };
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

        SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, nullptr);
        SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
        SDL_PushGPUVertexUniformData(commandBuffer, 0, &mvp, sizeof(mvp));
        SDL_GPUBufferBinding vertexBindings = { .buffer = vertexBuffer, .offset = 0 };
        SDL_BindGPUVertexBuffers(renderPass, 0, &vertexBindings, 1);
        SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);
        SDL_EndGPURenderPass(renderPass);
    }

    SDL_SubmitGPUCommandBuffer(commandBuffer);
    return 0;
}

void Demo001_Basics_BasicTriangle::Quit(Context* context) {
    SDL_ReleaseGPUGraphicsPipeline(context->device, pipeline);

    CommonQuit(context);
}

