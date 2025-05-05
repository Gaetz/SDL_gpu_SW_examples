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
    SDL_GPUShader* vertex_shader = LoadShader(context->device, context->base_path,
                                             "001_triangle.vert", 0, 1, 0, 0);
    if (vertex_shader == nullptr) {
        SDL_Log("Failed to create vertex shader!");
        return -1;
    }

    SDL_GPUShader* fragment_shader = LoadShader(context->device, context->base_path,
                                               "001_triangle.frag", 0, 0, 0, 0);
    if (fragment_shader == nullptr) {
        SDL_Log("Failed to create fragment shader!");
        return -1;
    }

    // Create the pipelines
    SDL_GPUGraphicsPipelineCreateInfo pipeline_create_info = {
            .vertex_shader = vertex_shader,
            .fragment_shader = fragment_shader,
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

    pipeline_create_info.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
    pipeline_ = SDL_CreateGPUGraphicsPipeline(context->device, &pipeline_create_info);
    if (pipeline_ == nullptr) {
        SDL_Log("Failed to create fill pipeline_!");
        return -1;
    }

    // Clean up shader resources
    SDL_ReleaseGPUShader(context->device, vertex_shader);
    SDL_ReleaseGPUShader(context->device, fragment_shader);

    // Create the vertex buffer
    SDL_GPUBufferCreateInfo vertex_buffer_create_info = {
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size = sizeof(PositionColorVertex) * 3
    };
    vertex_buffer_ = SDL_CreateGPUBuffer(context->device, &vertex_buffer_create_info);

    // To get data into the vertex buffer, we have to use a transfer buffer
    constexpr SDL_GPUTransferBufferCreateInfo transfer_buffer_create_info = {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = sizeof(PositionColorVertex) * 3
    };
    SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(context->device, &transfer_buffer_create_info);

    // Map the transfer buffer and fill it with data (data is bound to the transfer buffer)
    auto transferData = static_cast<PositionColorVertex*>(SDL_MapGPUTransferBuffer(context->device, transfer_buffer, false));
    transferData[0] = (PositionColorVertex) {  -0.5,  -0.5, 0, 255,   0,   0, 255 };
    transferData[1] = (PositionColorVertex) {   0.5,  -0.5, 0, 0, 255,   0, 255 };
    transferData[2] = (PositionColorVertex) {     0,   0.5, 0, 0, 0,   0, 255 };
    SDL_UnmapGPUTransferBuffer(context->device, transfer_buffer);

    // Upload the transfer data to the vertex buffer
    SDL_GPUTransferBufferLocation transfer_buffer_location = {
        .transfer_buffer = transfer_buffer,
        .offset = 0
    };
    SDL_GPUBufferRegion vertex_buffer_region = {
        .buffer = vertex_buffer_,
        .offset = 0,
        .size = sizeof(PositionColorVertex) * 3
    };

    // Upload the transfer data to the vertex buffer
    SDL_GPUCommandBuffer* upload_cmd_buf = SDL_AcquireGPUCommandBuffer(context->device);
    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(upload_cmd_buf);

    SDL_UploadToGPUBuffer(copy_pass, &transfer_buffer_location, &vertex_buffer_region, false);

    SDL_EndGPUCopyPass(copy_pass);
    SDL_SubmitGPUCommandBuffer(upload_cmd_buf);
    SDL_ReleaseGPUTransferBuffer(context->device, transfer_buffer);

    // Default MVP matrix
    mvp_ = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f) *
         glm::lookAt(glm::vec3(0, 0, -2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)) *
        glm::mat4(1.0f);
    return 0;
}

int Demo001_Basics_BasicTriangle::Update(Context* context)
{
    return 0;
}

int Demo001_Basics_BasicTriangle::Draw(Context* context) {
    SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(context->device);
    if (command_buffer == nullptr) {
        SDL_Log("AcquireGPUCommandBuffer failed: %s", SDL_GetError());
        return -1;
    }

    SDL_GPUTexture* swapchain_texture;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, context->window, &swapchain_texture, nullptr, nullptr)) {
        SDL_Log("WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
        return -1;
    }

    if (swapchain_texture != nullptr) {
        SDL_GPUColorTargetInfo color_target_info = { nullptr };
        color_target_info.texture = swapchain_texture;
        color_target_info.clear_color = SDL_FColor { 0.0f, 0.0f, 0.2f, 1.0f };
        color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
        color_target_info.store_op = SDL_GPU_STOREOP_STORE;

        SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(command_buffer, &color_target_info, 1, nullptr);
        SDL_BindGPUGraphicsPipeline(render_pass, pipeline_);
        SDL_PushGPUVertexUniformData(command_buffer, 0, &mvp_, sizeof(mvp_));
        SDL_GPUBufferBinding vertex_bindings = { .buffer = vertex_buffer_, .offset = 0 };
        SDL_BindGPUVertexBuffers(render_pass, 0, &vertex_bindings, 1);
        SDL_DrawGPUPrimitives(render_pass, 3, 1, 0, 0);
        SDL_EndGPURenderPass(render_pass);
    }

    SDL_SubmitGPUCommandBuffer(command_buffer);
    return 0;
}

void Demo001_Basics_BasicTriangle::Quit(Context* context) {
    SDL_ReleaseGPUGraphicsPipeline(context->device, pipeline_);

    CommonQuit(context);
}

