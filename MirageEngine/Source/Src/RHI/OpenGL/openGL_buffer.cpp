#include "OpenGL/openGL_buffer.h"
#include "OpenGL/openGL_pipeline.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace MirageRHI;

void OpenGLBuffer::Create(const void* _data, size_t _size, BufferType _type)
{
    m_Type = _type;

    switch (m_Type)
    {
    case IBuffer::Vertices:
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
        glBufferData(GL_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
        break;
    case IBuffer::Indices:
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
        break;
    case IBuffer::Texture:
        CreateTexture(_data, _size);
        break;
    default:
        DEBUG_THROW("Unknown Buffer Type Used!");
    }
}

void MirageRHI::OpenGLBuffer::Create(std::filesystem::path const& _filename, BufferType _type)
{
    m_Type = _type;

    switch (m_Type)
    {
    case IBuffer::Vertices:
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
        // Todo
        break;
    case IBuffer::Indices:
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
        // Todo
        break;
    case IBuffer::Texture:
        LoadTexture(_filename);
        break;
    default:
        DEBUG_THROW("Unknown Buffer Type Used!");
    }
}

void OpenGLBuffer::Destroy()
{
    if (m_Type == IBuffer::Vertices || m_Type == IBuffer::Indices)
        glDeleteBuffers(1, &m_Buffer);
    else if (m_Type == IBuffer::Texture)
        glDeleteTextures(1, &m_Buffer);
}

void OpenGLBuffer::Bind() const
{
    switch (m_Type)
    {
    case IBuffer::Vertices:
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
        break;
    case IBuffer::Indices:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
        break;
    case IBuffer::Texture:
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Buffer);
        break;
    default:
        DEBUG_THROW("Unknown Buffer Type Used!");
    }
}

void MirageRHI::OpenGLBuffer::CreateTexture(const void* _data, size_t _size)
{
    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &m_Buffer);
    glBindTexture(GL_TEXTURE_2D, m_Buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width = 0;
    int height = 0;
    int channel = 0;

    unsigned char* data = stbi_load_from_memory((const stbi_uc*)_data, (int)_size, &width, &height, &channel, 0);
    if (data) 
    {
        DEBUG_THROW("Texture Loading Error : data is incompatible !");
    }

    GLenum format = (channel == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void MirageRHI::OpenGLBuffer::LoadTexture(std::filesystem::path const& _filename)
{
    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &m_Buffer);
    glBindTexture(GL_TEXTURE_2D, m_Buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width = 0;
    int height = 0;
    int channel = 0;

    unsigned char* data = stbi_load(_filename.string().c_str(), &width, &height, &channel, 0);
    if (!data) 
    {
        DEBUG_THROW("Texture Loading Error : file %s is incompatible or doesn't exist !", _filename.string().c_str());
    }

    GLenum format = (channel == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}
