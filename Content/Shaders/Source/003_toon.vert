#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inColor;

layout (binding = 0, set = 1) uniform UBO
{
    mat4 mvp;
    vec4 lightPos;
} ubo;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outColor;
layout (location = 2) out vec3 outViewVec;
layout (location = 3) out vec3 outLightVec;

void main()
{
    outNormal = inNormal;
    outColor = inColor;
    gl_Position = ubo.mvp * vec4(inPos.xyz, 1.0);

    vec4 pos = ubo.mvp * vec4(inPos, 1.0);
    outNormal = mat3(ubo.mvp) * inNormal;
    vec3 lPos = mat3(ubo.mvp) * ubo.lightPos.xyz;
    outLightVec = lPos - pos.xyz;
    outViewVec = -pos.xyz;
}