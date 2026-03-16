#include "openGL/openGL_buffer.h"

void MirageRHI::OpenGLBuffer::Create(const void* data, size_t size)
{
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void MirageRHI::OpenGLBuffer::Destroy()
{
    glDeleteBuffers(1, &buffer);
}
