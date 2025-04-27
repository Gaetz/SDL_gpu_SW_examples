//
// Created by Gaetz on 16/04/2025.
//

#include "Demo001_Basics_BasicTriangle.hpp"
#include "Data.hpp"
#include "glm/mat4x4.hpp"

// Example with an instanced vertex buffer, if we donc want to use a big uniform
//https://github.com/TheSpydog/SDL_gpu_examples/blob/93d94af8a9008309e99ca1b6b415476685c66501/Examples/PullSpriteBatch.c

// Vertex shader code

/*
#version 450

struct SpriteData {
    vec3 Position;
    float Rotation;
    vec2 Scale;
    vec2 Padding;
    float TexU, TexV, TexW, TexH;
    vec4 Color;
};

layout(std430, binding = 0) readonly buffer DataBuffer {
    SpriteData sprites[];
};

layout(std140, binding = 1) uniform UniformBlock {
    mat4 ViewProjectionMatrix;
};

const uint triangleIndices[6] = uint[6](0, 1, 2, 3, 2, 1);
const vec2 vertexPos[4] = vec2[4](
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(0.0, 1.0),
    vec2(1.0, 1.0)
);

layout(location = 0) out vec2 Texcoord;
layout(location = 1) out vec4 Color;

void main() {
    uint id = gl_VertexID;
    uint spriteIndex = id / 6;
    uint vert = triangleIndices[id % 6];
    SpriteData sprite = sprites[spriteIndex];

    vec2 texcoord[4] = vec2[4](
        vec2(sprite.TexU,               sprite.TexV),
        vec2(sprite.TexU + sprite.TexW, sprite.TexV),
        vec2(sprite.TexU,               sprite.TexV + sprite.TexH),
        vec2(sprite.TexU + sprite.TexW, sprite.TexV + sprite.TexH)
    );

    float c = cos(sprite.Rotation);
    float s = sin(sprite.Rotation);

    vec2 coord = vertexPos[vert];
    coord *= sprite.Scale;
    mat2 rotation = mat2(c, s, -s, c);
    coord = rotation * coord;

    vec3 coordWithDepth = vec3(coord + sprite.Position.xy, sprite.Position.z);

    gl_Position = ViewProjectionMatrix * vec4(coordWithDepth, 1.0);
    Texcoord = texcoord[vert];
    Color = sprite.Color;
}
*/

int Demo001_Basics_BasicTriangle::Init(Context* context) {
    // Init SDL, window and device
    int result = CommonInit(context, 0);
    if (result < 0) { return result; }

    // Create the shaders
    SDL_GPUShader* vertexShader = LoadShader(context->device, context->basePath,
                                             "001_triangle.vert", 0, 0, 0, 0);
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

    // Configure the uniform buffer for set = 1
    SDL_GPUBufferCreateInfo uboCreateInfo = {
        .usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
        .size = sizeof(glm::mat4) * 3, // Space for projection, view, and model matrices
    };

    SDL_GPUBuffer* uniformBuffer = SDL_CreateGPUBuffer(context->device, &uboCreateInfo);
    if (!uniformBuffer) {
        SDL_Log("Failed to create uniform buffer: %s", SDL_GetError());
        return -1;
    }

    // Bind the uniform buffer to set = 1
    SDL_GPUBufferBinding uniformBinding = {
        .buffer = uniformBuffer,
        .offset = 0,
    };

    SDL_BindGPUVertexStorageBuffers(nullptr, 1, &uniformBinding, 1);

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
                                                                                .format = SDL_GPU_VERTEXELEMENTFORMAT_BYTE4,
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
    SDL_GPUBuffer* vertexBuffer = SDL_CreateGPUBuffer(context->device, &vertexBufferCreateInfo);

    // To get data into the vertex buffer, we have to use a transfer buffer
    constexpr SDL_GPUTransferBufferCreateInfo transferBufferCreateInfo = {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = sizeof(PositionColorVertex) * 3
    };
    SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(context->device, &transferBufferCreateInfo);

    // Map the transfer buffer and fill it with data (data is bound to the transfer buffer)
    auto transferData = static_cast<PositionColorVertex*>(SDL_MapGPUTransferBuffer(context->device, transferBuffer, false));
    transferData[0] = (PositionColorVertex) {  -0.5,  -0.5, 0, 255,   0,   0, 255 };
    transferData[1] = (PositionColorVertex) {   0.5,  -0.5, 0,   0, 255,   0, 255 };
    transferData[2] = (PositionColorVertex) {     0,   0.5, 0,   0,   0, 255, 255 };
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

    return 0;
}

int Demo001_Basics_BasicTriangle::Update(Context* context) { return 0; }

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

