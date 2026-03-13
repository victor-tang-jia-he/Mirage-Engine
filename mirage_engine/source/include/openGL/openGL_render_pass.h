#pragma once

#include "i_render_pass.h"

namespace MirageRHI
{
    // Clear Framebuffer before drawing
    class OpenGLRenderPass : public IRenderPass
    {
    private:
        float clearColor[4];
        float clearDepth = 1.0f;
        GLbitfield clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

    public:
        void Create(Vector4 color, float depth) override;
        void Destroy() override {}

        OpenGLRenderPass& API_OpenGL() override { return *this; }

        void Begin() override;
        void End() override {}
    };
}