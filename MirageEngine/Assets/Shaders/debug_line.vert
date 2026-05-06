#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;

uniform mat4 view;
uniform mat4 proj;

out vec4 Color;

void main()
{
    Color     = aColor;
    gl_Position = proj * view * vec4(aPosition, 1.0);
}