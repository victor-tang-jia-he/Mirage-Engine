#include "OpenGL/openGL_device.h"

GLuint MirageRHI::OpenGLDevice::CreateBuffer()
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    return buffer;
}

GLuint MirageRHI::OpenGLDevice::CreateTexture()
{
    GLuint tex;
    glGenTextures(1, &tex);
    return tex;
}
