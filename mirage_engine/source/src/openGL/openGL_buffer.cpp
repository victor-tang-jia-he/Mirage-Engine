#include "openGL/openGL_buffer.h"

void MirageRHI::OpenGLBuffer::Create(const void* _data, size_t _size)
{
    glGenBuffers(1, &m_Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
    glBufferData(GL_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
}

void MirageRHI::OpenGLBuffer::Destroy()
{
    glDeleteBuffers(1, &m_Buffer);
}
