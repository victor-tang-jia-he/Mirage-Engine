#pragma once

#include <glad/glad.h>

#include "i_render_pass.h"

namespace MirageRHI
{
    // Clear Framebuffer before drawing
    class OpenGLRenderPass : public IRenderPass
    {
    private:
        float m_ClearColor[4];
        float m_ClearDepth = 1.0f;
        GLbitfield m_ClearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

    public:
        void Create(Vector4 _color, float _depth) override;
        void Destroy() override {}

        OpenGLRenderPass& API_OpenGL() override { return *this; }

        void Begin() override;
        void End() override {}
    };
}