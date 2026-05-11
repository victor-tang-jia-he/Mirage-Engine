#pragma once

#include <i_swap_chain.h>

namespace MirageRHI
{
    class OpenGLSurface;
    class OpenGLFramebuffer;

    class OpenGLSwapChain : public ISwapChain
    {
        private:

            std::vector<OpenGLFramebuffer*> m_FrameBuffers;
            uint32_t currentFrame = 0;

        public:

            // Check and store GPU infos
            void Create(uint32_t _frameAmount, uint32_t _width, uint32_t _height) override;
            void Destroy() override;

            OpenGLSwapChain& API_OpenGL() override { return *this; }

            void IsScreenResized(std::shared_ptr<ISurface> _surface) override;
            void RecreateSwapchain(std::shared_ptr<ISurface> _surface, int _newWidth, int _newHeight) override;

            uint32_t AcquireCurrentFBO() override;
            uint32_t AcquireCurrentImage() override;
            void Present(OpenGLSurface _surface);
    };
} // namespace MirageRHI
