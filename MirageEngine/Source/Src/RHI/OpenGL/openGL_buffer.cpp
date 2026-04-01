#include "OpenGL/openGL_buffer.h"
#include "OpenGL/openGL_pipeline.h"
#include <stb_image.h>

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
    case BufferType::Texture:
        CreateTexture(_data, _size);
        break;
    default:
        DEBUG_THROW("Unknown Buffer Type Used!");
    }
}

void MirageRHI::OpenGLBuffer::Create(const std::filesystem::path& _filename, BufferType _type)
{
    m_Type = _type;

    switch (m_Type)
    {
    case BufferType::Vertices:
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
        // Todo
        break;
    case BufferType::Indices:
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
        // Todo
        break;
    case BufferType::Texture:
        //LoadTexture(_filename);
        break;
    default:
        DEBUG_THROW("Unknown Buffer Type Used!");
    }
}

void OpenGLBuffer::Destroy()
{
    if (m_Type == BufferType::Vertices || m_Type == BufferType::Indices)
        glDeleteBuffers(1, &m_Buffer);
    else if (m_Type == BufferType::Texture)
        glDeleteTextures(1, &m_Buffer);
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
    case BufferType::Texture:
       
        glBindTexture(GL_TEXTURE_2D, m_Buffer);
        break;
    default:
        DEBUG_THROW("Unknown Buffer Type Used!");
    }
}

void MirageRHI::OpenGLBuffer::CreateTexture(const void* _data, size_t _size)
{
    glGenTextures(1, &m_Buffer);
    glBindTexture(GL_TEXTURE_2D, m_Buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, _data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}