#pragma once

#include <i_command_buffer.h>

namespace MirageRHI
{
    class OpenGLCommandBuffer : public ICommandBuffer
    {
    public:
        // Wrapper so doesn't need initialisation
        void Create() override {}
        void Destroy() override {}

        OpenGLCommandBuffer& API_OpenGL() override { return *this; }

        // PlaceHolder Function (empty)
        void BeginDraw() override {}
        void EndDraw() override {}

        // Binding Functions
        void BindPipeline(IPipeline* _pipeline) override;
        void BindBuffer(IBuffer* _buffer) override;
        void BindTextureFromID(uint32_t _ID) override;

        void ApplyPipelineParameters(IPipeline* _pipeline) override;

        // Clear Framebuffers
        void BeginRenderPass(IRenderPass* _renderPass, uint32_t _FBO) override;
		void EndRenderPass() override {} // Nothing to do in OpenGL, as render pass is just a clear command.

        // Draw Binded Buffers
        void Draw(uint32_t _indexCount) override;

        // Swap Buffers
        void SubmitAndPresent(ISwapChain* _swapchain, ISurface* _surface) override;
    };
}