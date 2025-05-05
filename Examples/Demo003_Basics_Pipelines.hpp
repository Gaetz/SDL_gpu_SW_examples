//
// Created by gaetz on 05/05/25.
//

#ifndef DEMO003_BASICS_PIPELINES_HPP
#define DEMO003_BASICS_PIPELINES_HPP

#include "Scene.hpp"
#include <glm/ext/matrix_float4x4.hpp>


class Demo003_Basics_Pipelines : public Scene
{
public:
    int Init(const Context& context) override;
    void Update(const Context& context) override;
    void Draw(const Context& context) override;
    void Quit(const Context& context) override;

private:
    glm::mat4 mvp_{};
    glm::vec4 light_position_{0.0f, 2.0f, 1.0f, 0.0f};

    SDL_GPUGraphicsPipeline* phong_pipeline_ = nullptr;
    SDL_GPUGraphicsPipeline* toon_pipeline_ = nullptr;
    SDL_GPUGraphicsPipeline* wireframe_pipeline_ = nullptr;
    SDL_GPUBuffer* vertex_buffer_ = nullptr;
};


#endif //DEMO003_BASICS_PIPELINES_HPP
