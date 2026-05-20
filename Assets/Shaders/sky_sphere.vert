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

out vec4 FragPos;

void main()
{
    FragPos = vec4(aPosition, 1.0);
    mat4 newView = mat4(mat3(view));
    gl_Position = proj * newView * vec4(aPosition, 1.0);
}