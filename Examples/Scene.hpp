//
// Created by Gaetz on 16/04/2025.
//

#ifndef SCENE_HPP
#define SCENE_HPP

#include "Common.hpp"

class Scene
{
public:
    virtual int Init(Context* context) = 0;
    virtual int Update(Context* context) = 0;
    virtual int Draw(Context* context) = 0;
    virtual void Quit(Context* context) = 0;
}

#endif //SCENE_HPP
