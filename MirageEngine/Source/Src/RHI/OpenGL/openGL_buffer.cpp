#include "OpenGL/openGL_buffer.h"
#include "OpenGL/openGL_pipeline.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <Logs/log_macro.h>

DEFINE_LOG_CATEGORY(RHIBuffer)

using namespace MirageRHI;

void OpenGLBuffer::Create(const void* _data, size_t _size, BufferType _type)
{
    m_Type = _type;

    switch (m_Type)
    {
    case BufferType::Vertices:
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
        glBufferData(GL_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
        break;
    case BufferType::Indices:
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
        break;
    default:
        LOG(logWarning, RHIBuffer, "Unknown Buffer Type Used!");
    }

	LOG(logInfo, RHIBuffer, "Buffer Created with ID: ", m_Buffer);
}

void OpenGLBuffer::Destroy()
{
    glDeleteBuffers(1, &m_Buffer);

    LOG(logInfo, RHIBuffer, "Buffer Destroyed with ID: ", m_Buffer);
}

void OpenGLBuffer::Bind() const
{
    switch (m_Type)
    {
    case BufferType::Vertices:
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
        break;
    case BufferType::Indices:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
        break;
    default:
        LOG(logWarning, RHIBuffer, "Unknown Buffer Type Used!");
    }
}
