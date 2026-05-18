#pragma once

#include <i_uniform_buffer.h>

#include <Components/camera.h>

namespace MirageRHI
{
    class OpenGLUniformBuffer : public IUniformBuffer
    {
    private:
        unsigned int m_UniformBufferId = 0;
        UBOBinding m_Binding;

    public:
        ~OpenGLUniformBuffer() = default;
        void Create(UBOBinding _binding, size_t _size) override;
        void Upload(const void* _data, size_t _size) override;
        void Destroy() override {};

        int GetID() override { return m_UniformBufferId; }

        virtual OpenGLUniformBuffer& API_OpenGL() { return *this; }
    };
} // namespace MirageRHI
