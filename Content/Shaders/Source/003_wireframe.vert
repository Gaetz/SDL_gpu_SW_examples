#version 450

layout (location = 0) in vec4 inPos;
layout (location = 2) in vec3 inColor;

layout (binding = 0, set = 1) uniform UBO
{
    mat4 mvp;
} ubo;

layout (location = 0) out vec3 outColor;

void main()
{
    outColor = inColor;
    gl_Position = ubo.mvp * inPos;
}
