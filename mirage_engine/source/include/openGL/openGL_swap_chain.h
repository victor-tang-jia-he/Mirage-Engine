#pragma once

#include "i_swap_chain.h"

namespace MirageRHI
{
    class OpenGLSwapChain : public ISwapChain
    {
    public:
        // Check and store GPU infos
        void Create() override {}
        void Destroy() override {}

        OpenGLSwapChain& API_OpenGL() override { return *this; }

        void IsScreenResized(ISurface* surface) override;
        void RecreateSwapchain(ISurface* surface, int newWidth, int newHeight) override;

        uint32_t AcquireNextImage() override { return 0; }
        void Present(OpenGLSurface surface);
    };
}