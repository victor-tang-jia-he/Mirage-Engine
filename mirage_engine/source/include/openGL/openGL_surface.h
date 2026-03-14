#pragma once

#include "i_surface.h"

namespace MirageRHI
{
    // Stores glfw context (window)
    class OpenGLSurface : public ISurface
    {
    private:
        GLFWwindow* context = nullptr;
        int width;
        int height;

    public:
        void Create(GLFWwindow* _context, int _width, int _height) override;
        void Destroy() override {}

        OpenGLSurface& API_OpenGL() override { return *this; }
        GLFWwindow* GetContextGLHandle() const { return context; }
        int GetWidth() const { return width; }
        int GetHeight() const { return height; }

        void SetSize(int _width, int _height);
    };
}