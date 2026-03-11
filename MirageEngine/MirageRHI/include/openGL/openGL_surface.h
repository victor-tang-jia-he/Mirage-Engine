#pragma once

#include "..\i_surface.h"

namespace MirageRHI
{
    class OpenGLSurface : public ISurface
    {
    private:
        GLFWwindow* window = nullptr;

        int width;
        int height;

    public:
        void Create(int w, int h, const char* title) override;
        void Destroy() override;

        OpenGLSurface& API_OpenGL() override { return *this; }
        GLFWwindow* GetWindowGLHandle() const { return window; }
    };
}