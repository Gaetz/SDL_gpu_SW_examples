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
    int Init(Context* context) override;
    int Update(Context* context) override;
    int Draw(Context* context) override;
    void Quit(Context* context) override;

private:
    SDL_GPUGraphicsPipeline* pipeline = nullptr;
    SDL_GPUBuffer* vertexBuffer = nullptr;
    glm::mat4 mvp = {};
};

#endif //DEMO001BASICSBASICTRIANGLE_HPP
