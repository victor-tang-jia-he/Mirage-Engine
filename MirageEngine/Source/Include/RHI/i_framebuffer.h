#pragma once

#include <iostream>

DECLARE_LOG_CATEGORY_EXTERN(LogRHIFramebuffer)

namespace MirageRHI
{
    class OpenGLFramebuffer;

    class IFramebuffer
    {
    public:

        virtual ~IFramebuffer() = default;

        virtual void Create(uint32_t _width, uint32_t _height) = 0;
        virtual void Destroy() = 0;

        virtual uint32_t GetFrameTextureID() = 0;

        virtual OpenGLFramebuffer& API_OpenGL() { throw std::runtime_error("Bad API Call : object is not an OpenGLFramebuffer"); }
    };
} // namespace MirageRHI
