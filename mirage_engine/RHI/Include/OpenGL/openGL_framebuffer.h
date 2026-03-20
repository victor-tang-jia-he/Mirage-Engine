#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "i_framebuffer.h"

namespace MirageRHI
{
    class OpenGLFramebuffer : public IFramebuffer
    {
    private:
        GLuint m_FBO = 0;

    public:
        void Create() override;
        void Destroy() override;

        void Bind();

        OpenGLFramebuffer& API_OpenGL() override { return *this; }
    };
}