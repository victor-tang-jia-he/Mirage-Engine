#pragma once

#include "i_command_buffer.h"

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
        void BindPipeline(IPipeline* pipeline) override;
        void BindBuffers(IBuffer* indexBuffer, IBuffer* vertexBuffer, IPipeline* pipeline) override;
        void BindVertexBuffer(OpenGLBuffer buffer);
        void BindIndexBuffer(OpenGLBuffer buffer);
        void SetVertAttrLayout(OpenGLPipeline pipeline);

        // Clear Framebuffers
        void BeginRenderPass(IRenderPass* renderPass) override;
        void EndRenderPass() override {}

        // Draw Binded Buffers
        void DrawIndexed(uint32_t indexCount) override;

        // Swap Buffers
        void SubmitAndPresent(ISwapChain* swapchain, ISurface* surface) override;
    };
}