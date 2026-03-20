#include "OpenGL/openGL_buffer.h"
#include <OpenGL/openGL_pipeline.h>

void MirageRHI::OpenGLBuffer::Create(const void* _data, size_t _size, BufferType _type)
{
    m_Type = _type;

    switch (m_Type)
    {
    case MirageRHI::IBuffer::Vertices:
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
        glBufferData(GL_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
        break;
    case MirageRHI::IBuffer::Indices:
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
        break;
    case MirageRHI::IBuffer::Texture:
        glGenTextures(1, &m_Buffer);
        break;
    default:
        DEBUG_THROW("Unknown Buffer Type Used!");
    }
}

void MirageRHI::OpenGLBuffer::Destroy()
{
    glDeleteBuffers(1, &m_Buffer);
}

void MirageRHI::OpenGLBuffer::Bind() const
{
    switch (m_Type)
    {
    case MirageRHI::IBuffer::Vertices:
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
        break;
    case MirageRHI::IBuffer::Indices:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
        break;
    case MirageRHI::IBuffer::Texture:
        // TODO
        break;
    default:
        DEBUG_THROW("Unknown Buffer Type Used!");
    }
}
