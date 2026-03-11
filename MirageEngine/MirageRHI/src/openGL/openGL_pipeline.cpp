#include "openGL_pipeline.h"
#include "openGL_pipeline.h"
#include "../../include/openGL/openGL_pipeline.h"

void MirageRHI::OpenGLPipeline::Create(const char* vertSrc, const char* fragSrc)
{
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertSrc, nullptr);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragSrc, nullptr);
    glCompileShader(fragmentShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGenVertexArrays(1, &vao);
}

void MirageRHI::OpenGLPipeline::Destroy()
{
    glDeleteProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteVertexArrays(1, &vao);
}
