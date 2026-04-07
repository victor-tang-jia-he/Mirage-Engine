#pragma once

#include <i_buffer.h>

namespace MirageRHI
{
	class OpenGLBuffer : public IBuffer
	{
    private:
        uint32_t m_Buffer = 0;
        BufferType m_Type = BufferType::None;

    public:
        // Creates and stores buffer data (vertexes, indices)
        void Create(const void* _data, size_t _size, BufferType _type) override;
        void Destroy() override;

        // Getters
        OpenGLBuffer& API_OpenGL() override { return *this; }
        uint32_t GetGLHandle() const { return m_Buffer; }

        void Bind() const override;
	};
}