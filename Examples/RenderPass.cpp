//
// Created by gaetz on 05/05/25.
//

#include "RenderPass.hpp"

#include "Buffer.hpp"
#include "GraphicsPipeline.hpp"

void RenderPass::AddColorTarget(SDL_GPUTexture* swapchain_texture, const SDL_FColor& clear_color,
                                LoadOperation load_op, StoreOperation store_op)
{
    SDL_GPUColorTargetInfo color_target_info{};
    color_target_info.texture = swapchain_texture;
    color_target_info.clear_color = clear_color;
    color_target_info.load_op = static_cast<SDL_GPULoadOp>(load_op);
    color_target_info.store_op = static_cast<SDL_GPUStoreOp>(store_op);
    color_targets_.push_back(color_target_info);
}

void RenderPass::Begin(SDL_GPUCommandBuffer* command_buffer)
{
    render_pass_ = SDL_BeginGPURenderPass(command_buffer, color_targets_.data(), static_cast<u32>(color_targets_.size()),
                              depth_stencil_target_);
}

void RenderPass::DrawPrimitives(const u32 num_vertices, const u32 num_instances, const u32 first_vertex, const u32 first_instance) const
{
    SDL_DrawGPUPrimitives(render_pass_, num_vertices, num_instances, first_vertex, first_instance);
}

void RenderPass::End()
{
    SDL_EndGPURenderPass(render_pass_);
    color_targets_.clear();
    depth_stencil_target_ = nullptr;
}

void RenderPass::BindPipeline(const GraphicsPipeline& pipeline) const
{
    SDL_BindGPUGraphicsPipeline(render_pass_, pipeline.GetPipelinePtr());
}

void RenderPass::BindVertexBuffer(const Buffer& buffer, const u32 first_slot) const
{
    const Vector<SDL_GPUBufferBinding>& vertex_bindings = buffer.GetVertexBindings();
    SDL_BindGPUVertexBuffers(render_pass_, first_slot, vertex_bindings.data(), static_cast<u32>(vertex_bindings.size()));
}
