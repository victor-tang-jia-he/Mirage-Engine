#version 330 core

#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main() {
    vec4 worldPos = vec4(aPos, 1.0) * model;
    FragPos = vec3(worldPos);
    Normal = aNormal * mat3(transpose(inverse(model)));
    TexCoord = aTexCoord;
    gl_Position = proj * view * vec4(FragPos, 1.0);
}
