#pragma once

#include "..\i_surface.h"

namespace MirageRHI
{
    class OpenGLSurface : public ISurface
    {
    private:
        GLFWwindow* context = nullptr;

    public:
        void Create(GLFWwindow* _context) override;
        void Destroy() override {}

        OpenGLSurface& API_OpenGL() override { return *this; }
        GLFWwindow* GetContextGLHandle() const { return context; }
    };
}