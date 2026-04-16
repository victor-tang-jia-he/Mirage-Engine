#include "OpenGL/openGL_buffer.h"
#include "OpenGL/openGL_pipeline.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

DEFINE_LOG_CATEGORY(LogRHIBuffer)

using namespace MirageRHI;

void OpenGLBuffer::Create(const void* _data, size_t _size, BufferType _type)
{
    m_Type = _type;

    switch (m_Type)
    {
    case BufferType::Vertices:
        glGenVertexArrays(1, &m_Vao);
        glBindVertexArray(m_Vao);

        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
        glBufferData(GL_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
        break;

    case BufferType::Indices:
        glBindVertexArray(m_Vao);

        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
        break;
    default:
        LOG(logWarning, LogRHIBuffer, "Unknown Buffer Type Used!");
    }

	LOG(logTemp, LogRHIBuffer, "Buffer Created with ID: ", m_Buffer);
}

void OpenGLBuffer::Destroy()
{
    glDeleteBuffers(1, &m_Buffer);

    LOG(logTemp, LogRHIBuffer, "Buffer Destroyed with ID: ", m_Buffer);
}

void OpenGLBuffer::Bind() const
{
    switch (m_Type)
    {
    case BufferType::Vertices:
        glBindVertexArray(m_Vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);

        glVertexAttribPointer(
            0,                  
            3,                  
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 8,
            (void*)0
        );
        glEnableVertexAttribArray(0);
        break;

    case BufferType::Indices:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
        break;
    }
}
