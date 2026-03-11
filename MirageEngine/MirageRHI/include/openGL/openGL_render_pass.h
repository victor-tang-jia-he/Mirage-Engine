#pragma once

#include "..\i_render_pass.h"

namespace MirageRHI
{
    class OpenGLRenderPass : public IRenderPass
    {
    private:
        float clearColor[4];
        float clearDepth = 1.0f;
        GLbitfield clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

    public:
        void Create(float r, float g, float b, float a, float depth) override; // TODO: Change floats to quaternions
        void Destroy() override {}

        OpenGLRenderPass& API_OpenGL() override { return *this; }

        void Begin() override;
        void End() override {}
    };
}