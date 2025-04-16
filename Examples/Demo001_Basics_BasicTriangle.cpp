//
// Created by Gaetz on 16/04/2025.
//

#include "Demo001_Basics_BasicTriangle.hpp"

int Demo001_Basics_BasicTriangle::Init(Context* context) {
    // Init SDL, window and device
    int result = CommonInit(context, 0);
    if (result < 0) { return result; }

    // Create the shaders
    SDL_GPUShader* vertexShader = LoadShader(context->device, "SaschaWillemsVulkanDemo/001_triangle.vert", 0, 0, 0, 0);
    if (vertexShader == NULL)
    {
        SDL_Log("Failed to create vertex shader!");
        return -1;
    }

    SDL_GPUShader* fragmentShader = LoadShader(context->device, "SaschaWillemsVulkanDemo/001_triangle.frag", 0, 0, 0, 0);
    if (fragmentShader == NULL)
    {
        SDL_Log("Failed to create fragment shader!");
        return -1;
    }

    // Create the pipelines
    SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {
        .vertex_shader = vertexShader,
        .fragment_shader = fragmentShader,
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .vertex_input_state = (SDL_GPUVertexInputState){
            .num_vertex_buffers = 1,
            .vertex_buffer_descriptions = (SDL_GPUVertexBufferDescription[]){{
                .slot = 0,
                .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                .instance_step_rate = 0,
                .pitch = sizeof(PositionColorVertex)
            }},
            .num_vertex_attributes = 2,
            .vertex_attributes = (SDL_GPUVertexAttribute[]){{
                .buffer_slot = 0,
                .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                .location = 0,
                .offset = 0
            }, {
                .buffer_slot = 0,
                .format = SDL_GPU_VERTEXELEMENTFORMAT_BYTE4,
                .location = 1,
                .offset = sizeof(char) * 4
            }}
        },
        .target_info = {
            .color_target_descriptions = (SDL_GPUColorTargetDescription[]) {
                {
                    .format = SDL_GetGPUSwapchainTextureFormat(context->Device, context->Window)
                }
            },
            .num_color_targets = 1,
        },

    };

    pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
    pipeline = SDL_CreateGPUGraphicsPipeline(context->Device, &pipelineCreateInfo);
    if (pipeline == NULL)
    {
        SDL_Log("Failed to create fill pipeline!");
        return -1;
    }

    // Clean up shader resources
    SDL_ReleaseGPUShader(context->Device, vertexShader);
    SDL_ReleaseGPUShader(context->Device, fragmentShader);

    return 0;
}

int Demo001_Basics_BasicTriangle::Update(Context* context) { return 0; }

int Demo001_Basics_BasicTriangle::Draw(Context* context) {
    SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(context->Device);
    if (commandBuffer == NULL)
    {
        SDL_Log("AcquireGPUCommandBuffer failed: %s", SDL_GetError());
        return -1;
    }

    SDL_GPUTexture* swapchainTexture;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, context->Window, &swapchainTexture, NULL, NULL))
    {
        SDL_Log("WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
        return -1;
    }

    if (swapchainTexture != NULL)
    {
        SDL_GPUColorTargetInfo colorTargetInfo = {0};
        colorTargetInfo.texture = swapchainTexture;
        colorTargetInfo.clear_color = (SDL_FColor){ 0.0f, 0.0f, 0.2f, 1.0f };
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

        SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, NULL);
        SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
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
