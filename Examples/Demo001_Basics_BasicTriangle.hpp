//
// Created by Gaetz on 16/04/2025.
//

#ifndef DEMO001BASICSBASICTRIANGLE_HPP
#define DEMO001BASICSBASICTRIANGLE_HPP

#include <glm/ext/matrix_float4x4.hpp>

#include "Buffer.hpp"
#include "GraphicsPipeline.hpp"
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

    GraphicsPipeline pipeline_{};
    //SDL_GPUBuffer* vertex_buffer_ = nullptr;
    Buffer vertex_buffer_{};
};

#endif //DEMO001BASICSBASICTRIANGLE_HPP
