#pragma once

#include <GLFW/glfw3.h>

#include <i_swap_chain.h>

namespace MirageRHI
{
    class OpenGLSurface;
}

namespace MirageRHI
{
    class OpenGLSwapChain : public ISwapChain
    {
    public:
        // Check and store GPU infos
        void Create() override {}
        void Destroy() override {}

        OpenGLSwapChain& API_OpenGL() override { return *this; }

        void IsScreenResized(ISurface* _surface) override;
        void RecreateSwapchain(ISurface* _surface, int _newWidth, int _newHeight) override;

        uint32_t AcquireNextImage() override { return 0; }
        void Present(OpenGLSurface _surface);
    };
}