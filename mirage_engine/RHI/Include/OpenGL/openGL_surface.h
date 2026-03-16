#pragma once

#include "i_surface.h"

namespace MirageRHI
{
    // Stores glfw context (window)
    class OpenGLSurface : public ISurface
    {
    private:
        GLFWwindow* m_Context = nullptr;
        int m_Width;
        int m_Height;

    public:
        void Create(GLFWwindow* _context, int _width, int _height) override;
        void Destroy() override {}

        OpenGLSurface& API_OpenGL() override { return *this; }
        GLFWwindow* GetContextGLHandle() const { return m_Context; }
        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }

        void SetSize(int _width, int _height);
    };
}