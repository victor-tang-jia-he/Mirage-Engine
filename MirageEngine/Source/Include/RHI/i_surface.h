#pragma once

#include <iostream>

DECLARE_LOG_CATEGORY_EXTERN(LogRHISurface)

struct GLFWwindow;

namespace MirageRHI
{
    class OpenGLSurface;

    class ISurface
    {
    public:

        virtual ~ISurface() = default;

        virtual void Create(GLFWwindow* _context, int _width, int _height) = 0;
        virtual void Destroy() = 0;

        virtual OpenGLSurface& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLSurface"); }
    };
} // namespace MirageRHI
