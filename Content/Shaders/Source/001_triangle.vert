#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inColor;


layout (set = 1, binding = 0) uniform UBO
{
	mat4 mvp;
} ubo;


layout (location = 0) out vec3 outColor;

void main()
{
	outColor = inColor.rgb;
	gl_Position = ubo.mvp * vec4(inPos.xyz, 1.0);
}
