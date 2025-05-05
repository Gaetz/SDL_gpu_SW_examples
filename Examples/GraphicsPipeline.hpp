//
// Created by gaetz on 05/05/25.
//

#ifndef GRAPHICS_PIPELINE_HPP
#define GRAPHICS_PIPELINE_HPP
#include "Defines.hpp"
#include "Shader.hpp"
#include "SDL3/SDL_gpu.h"

enum class PrimitiveType
{
    TriangleList = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
    TriangleStrip = SDL_GPU_PRIMITIVETYPE_TRIANGLESTRIP,
    LineList = SDL_GPU_PRIMITIVETYPE_LINELIST,
    LineStrip = SDL_GPU_PRIMITIVETYPE_LINESTRIP,
    PointList = SDL_GPU_PRIMITIVETYPE_POINTLIST
};

enum class FillMode
{
    Fill = SDL_GPU_FILLMODE_FILL,
    Line = SDL_GPU_FILLMODE_LINE
};

enum class CullMode
{
    None = SDL_GPU_CULLMODE_NONE,
    Front = SDL_GPU_CULLMODE_FRONT,
    Back = SDL_GPU_CULLMODE_BACK
};

enum class FrontFace
{
    CounterClockwise = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE,
    Clockwise = SDL_GPU_FRONTFACE_CLOCKWISE
};

class GraphicsPipeline
{
public:
    GraphicsPipeline() = default;
    ~GraphicsPipeline();

    SDL_GPUGraphicsPipeline* Create();
    void Bind(SDL_GPURenderPass*) const;
    void Release();

    void SetDevice(SDL_GPUDevice* device) { device_ = device; }
    void SetVertexShader(const Shader& shader);
    void SetFragmentShader(const Shader& shader);
    void SetVertexInputState(const Vector<SDL_GPUVertexBufferDescription>& vertex_buffer_descriptions,
                             const Vector<SDL_GPUVertexAttribute>& vertex_attributes);
    void SetPrimitiveType(PrimitiveType primitive_type);
    void SetRasterizerState(FillMode fill_mode = FillMode::Fill,
                            CullMode cull_mode = CullMode::Back,
                            FrontFace front_face = FrontFace::CounterClockwise,
                            bool enable_depth_bias = false,
                            float depth_bias_constant_factor = 0.0f,
                            float depth_bias_clamp = 0.0f,
                            float depth_bias_slope_factor = 0.0f,
                            bool enable_depth_clip = false
    );
    void SetTargetInfo(const Vector<SDL_GPUColorTargetDescription>& color_target_descriptions,
                       SDL_GPUTextureFormat depth_stencil_format = SDL_GPU_TEXTUREFORMAT_INVALID);

private:
    SDL_GPUDevice* device_ = nullptr;
    SDL_GPUGraphicsPipeline* pipeline_ = nullptr;

    SDL_GPUGraphicsPipelineCreateInfo pipeline_create_info_{};
};


#endif //GRAPHICS_PIPELINE_HPP
