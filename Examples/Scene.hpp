//
// Created by Gaetz on 16/04/2025.
//

#ifndef SCENE_HPP
#define SCENE_HPP

#include "Common.hpp"

class Scene
{
public:
    virtual ~Scene() = default;

    virtual int Init(const Context& context) = 0;
    virtual void Update(const Context& context) = 0;
    virtual void Draw(const Context& context) = 0;
    virtual void Quit(const Context& context) = 0;
};

#endif //SCENE_HPP
