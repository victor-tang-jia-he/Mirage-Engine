#pragma once

#include "i_framebuffer.h"

namespace MirageRHI
{
    class OpenGLFramebuffer : public IFramebuffer
    {
    private:
        GLuint m_FBO;

    public:
        void Create() override;
        void Destroy() override;

        void Bind();

        OpenGLFramebuffer& API_OpenGL() override { return *this; }
    };
}