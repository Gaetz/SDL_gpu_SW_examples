//
// Created by gaetz on 05/05/25.
//

#include "GraphicsPipeline.hpp"

#include "RenderPass.hpp"
#include "SDL3/SDL_log.h"

GraphicsPipeline::~GraphicsPipeline()
{
    Release();
}

SDL_GPUGraphicsPipeline* GraphicsPipeline::Create()
{
    pipeline_ = SDL_CreateGPUGraphicsPipeline(device_, &pipeline_create_info_);
    if (pipeline_ == nullptr)
    {
        SDL_Log("Failed to create pipeline!");
        return nullptr;
    }
    return pipeline_;
}

void GraphicsPipeline::Release()
{
    if (pipeline_ != nullptr)
    {
        SDL_ReleaseGPUGraphicsPipeline(device_, pipeline_);
        pipeline_ = nullptr;
    }
}

void GraphicsPipeline::SetVertexShader(const Shader& shader)
{
    pipeline_create_info_.vertex_shader = shader.GetShaderPtr();
}

void GraphicsPipeline::SetFragmentShader(const Shader& shader)
{
    pipeline_create_info_.fragment_shader = shader.GetShaderPtr();
}

void GraphicsPipeline::SetVertexInputState(const Vector<SDL_GPUVertexBufferDescription>& vertex_buffer_descriptions,
                                           const Vector<SDL_GPUVertexAttribute>& vertex_attributes)
{
    pipeline_create_info_.vertex_input_state.vertex_buffer_descriptions = vertex_buffer_descriptions.data();
    pipeline_create_info_.vertex_input_state.num_vertex_buffers =
        static_cast<uint32_t>(vertex_buffer_descriptions.size());
    pipeline_create_info_.vertex_input_state.vertex_attributes = vertex_attributes.data();
    pipeline_create_info_.vertex_input_state.num_vertex_attributes = static_cast<uint32_t>(vertex_attributes.size());
}

void GraphicsPipeline::SetPrimitiveType(PrimitiveType primitive_type)
{
    pipeline_create_info_.primitive_type = static_cast<SDL_GPUPrimitiveType>(primitive_type);
}

void GraphicsPipeline::SetRasterizerState(FillMode fill_mode, CullMode cull_mode, FrontFace front_face,
                                          const bool enable_depth_bias,
                                          const float depth_bias_constant_factor, const float depth_bias_clamp,
                                          const float depth_bias_slope_factor, const bool enable_depth_clip)
{
    pipeline_create_info_.rasterizer_state.fill_mode = static_cast<SDL_GPUFillMode>(fill_mode);
    pipeline_create_info_.rasterizer_state.cull_mode = static_cast<SDL_GPUCullMode>(cull_mode);
    pipeline_create_info_.rasterizer_state.front_face = static_cast<SDL_GPUFrontFace>(front_face);
    pipeline_create_info_.rasterizer_state.enable_depth_bias = enable_depth_bias;
    pipeline_create_info_.rasterizer_state.depth_bias_constant_factor = depth_bias_constant_factor;
    pipeline_create_info_.rasterizer_state.depth_bias_clamp = depth_bias_clamp;
    pipeline_create_info_.rasterizer_state.depth_bias_slope_factor = depth_bias_slope_factor;
    pipeline_create_info_.rasterizer_state.enable_depth_clip = enable_depth_clip;
}

void GraphicsPipeline::SetTargetInfo(const Vector<SDL_GPUColorTargetDescription>& color_target_descriptions,
                                     const SDL_GPUTextureFormat depth_stencil_format)
{
    pipeline_create_info_.target_info.color_target_descriptions = color_target_descriptions.data();
    pipeline_create_info_.target_info.num_color_targets = static_cast<uint32_t>(color_target_descriptions.size());
    pipeline_create_info_.target_info.depth_stencil_format = depth_stencil_format;
    if (depth_stencil_format != SDL_GPU_TEXTUREFORMAT_INVALID)
    {
        pipeline_create_info_.target_info.has_depth_stencil_target = true;
    }
    else
    {
        pipeline_create_info_.target_info.has_depth_stencil_target = false;
    }
}
