//
// Created by gaetz on 05/05/25.
//

#ifndef RENDERPASS_HPP
#define RENDERPASS_HPP
#include "Common.hpp"
#include "Defines.hpp"
#include "SDL3/SDL_gpu.h"

class RenderPass {
public:
    RenderPass() = default;

    SDL_GPURenderPass* GetRenderPassPointer() const { return render_pass_; }


    void AddColorTarget(SDL_GPUTexture *swapchain_texture, const SDL_FColor& clear_color,
                        LoadOperation load_op, StoreOperation store_op);
    void Begin(SDL_GPUCommandBuffer *command_buffer);
    void DrawPrimitives(u32 num_vertices, u32 num_instances, u32 first_vertex, u32 first_instance) const;
    void End();

    void BindPipeline(const class GraphicsPipeline& pipeline) const;
    void BindVertexBuffer(const class Buffer& buffer, const u32 first_slot) const;



private:
    SDL_GPURenderPass* render_pass_{nullptr};
    Vector<SDL_GPUColorTargetInfo> color_targets_{};
    SDL_GPUDepthStencilTargetInfo* depth_stencil_target_{nullptr};
};



#endif //RENDERPASS_HPP
