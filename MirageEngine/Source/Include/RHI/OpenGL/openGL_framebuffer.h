#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <i_framebuffer.h>

namespace MirageRHI
{
    class OpenGLFramebuffer : public IFramebuffer
    {
    private:
        uint32_t m_FBO = 0;
        uint32_t m_Texture = 0;
        uint32_t m_RBO = 0;

        uint32_t m_Width = 0;
        uint32_t m_Height = 0;

    public:
        void Create(uint32_t _width, uint32_t _height) override;
        void Destroy() override;

        OpenGLFramebuffer& API_OpenGL() override { return *this; }
        uint32_t GetOpenGLHandle() const { return m_FBO; }
        uint32_t GetFrameTexture() const override { return m_Texture; }

        void RescaleFrameBuffer(uint32_t _width, uint32_t _height) const;
        void Bind() const;
        void Unbind() const;
    };
}