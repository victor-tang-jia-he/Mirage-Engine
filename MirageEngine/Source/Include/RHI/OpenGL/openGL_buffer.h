#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <debug.h>

#include <i_buffer.h>

namespace MirageRHI
{
	class OpenGLBuffer : public IBuffer
	{
    private:
        GLuint m_Buffer = 0;
        BufferType m_Type = BufferType::None;

    public:
        // Creates and stores buffer data (vertexes, indices, textures, ...)
        void Create(const void* _data, size_t _size, BufferType _type) override;
        void Create(std::filesystem::path const& _filename, BufferType _type) override;
        void Destroy() override;

        OpenGLBuffer& API_OpenGL() override { return *this; }

        void Bind() const;

        // Getters
        GLuint GetGLHandle() const { return m_Buffer; }

    private:
        void CreateTexture(const void* _data, size_t _size);

        void LoadTexture(std::filesystem::path const& _filename);
	};
}