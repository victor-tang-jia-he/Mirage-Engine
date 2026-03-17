#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <debug.h>

#include "i_buffer.h"

namespace MirageRHI
{
	class OpenGLBuffer : public IBuffer
	{
    private:
        GLuint m_Buffer;
        BufferType m_Type;

    public:
        // Creates and stores buffer data (vertexes, indices, textures, ...)
        void Create(const void* _data, size_t _size, BufferType _type) override;
        void Destroy() override;

        OpenGLBuffer& API_OpenGL() override { return *this; }

        void SetLayout() const;

        // Getters
        GLuint GetGLHandle() const { return m_Buffer; }
	};
}