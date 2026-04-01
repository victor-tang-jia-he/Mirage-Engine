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
        void Create(const std::filesystem::path& _filename, BufferType _type) override;
        void Destroy() override;

        // Getters
        OpenGLBuffer& API_OpenGL() override { return *this; }
        GLuint GetGLHandle() const { return m_Buffer; }

        void Bind() const override;

    private:
        void CreateTexture(const void* _data, size_t _size);

        void LoadTexture(const std::filesystem::path& _filename);
	};
}