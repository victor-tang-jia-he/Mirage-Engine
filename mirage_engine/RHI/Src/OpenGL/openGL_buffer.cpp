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
    case MirageRHI::IBuffer::VertexAttributes:
        glGenVertexArrays(1, &m_Buffer);
        glBindVertexArray(m_Buffer);
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

void MirageRHI::OpenGLBuffer::SetLayout() const
{
    if (m_Type != BufferType::VertexAttributes)
        return;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
}
