#pragma once

#include "i_buffer.h"

namespace MirageRHI
{
	class OpenGLBuffer : public IBuffer
	{
    private:
        GLuint m_Buffer;
        // TODO : Add enum different buffer types;
    public:
        // Creates and stores buffer data (vertexes, indices, textures, ...)
        void Create(const void* _data, size_t _size) override;
        void Destroy() override;

        OpenGLBuffer& API_OpenGL() override { return *this; }
        GLuint GetGLHandle() const { return m_Buffer; }
	};
}