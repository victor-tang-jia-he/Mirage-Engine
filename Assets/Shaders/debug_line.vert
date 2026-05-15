#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;

layout(std140) uniform PerFrame
{
    mat4 view;
    mat4 proj;
    vec3 camPos;
    float padding;
};

out vec4 Color;

void main()
{
    Color     = aColor;
    gl_Position = proj * view * vec4(aPosition, 1.0);
}