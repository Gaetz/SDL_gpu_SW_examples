//
// Created by Gaetz on 16/04/2025.
//

#ifndef DEMO001BASICSBASICTRIANGLE_HPP
#define DEMO001BASICSBASICTRIANGLE_HPP

#include <glm/ext/matrix_float4x4.hpp>

#include "Scene.hpp"

class Demo001_Basics_BasicTriangle : public Scene
{
public:
    int Init(const Context& context) override;
    void Update(const Context& context) override;
    void Draw(const Context& context) override;
    void Quit(const Context& context) override;

private:
    glm::mat4 mvp_{};

    SDL_GPUGraphicsPipeline* pipeline_ = nullptr;
    SDL_GPUBuffer* vertex_buffer_ = nullptr;
};

#endif //DEMO001BASICSBASICTRIANGLE_HPP
