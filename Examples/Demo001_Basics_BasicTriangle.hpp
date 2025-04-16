//
// Created by Gaetz on 16/04/2025.
//

#ifndef DEMO001BASICSBASICTRIANGLE_HPP
#define DEMO001BASICSBASICTRIANGLE_HPP

#include "Scene.hpp"

class Demo001_Basics_BasicTriangle : public Scene
{
public:
    int Init(Context* context) override;
    int Update(Context* context) override;
    int Draw(Context* context) override;
    void Quit(Context* context) override;

private:
    SDL_GPUGraphicsPipeline* pipeline;
};



#endif //DEMO001BASICSBASICTRIANGLE_HPP
