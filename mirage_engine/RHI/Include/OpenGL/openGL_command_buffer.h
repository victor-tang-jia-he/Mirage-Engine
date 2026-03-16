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
        void BindPipeline(IPipeline* _pipeline) override;
        void BindBuffers(IBuffer* _indexBuffer, IBuffer* _vertexBuffer, IBuffer* _VAO) override;
        void BindVertexBuffer(OpenGLBuffer _buffer);
        void BindIndexBuffer(OpenGLBuffer _buffer);
        void SetVertAttrLayout(OpenGLBuffer _pipeline);

        // Clear Framebuffers
        void BeginRenderPass(IRenderPass* _renderPass) override;
        void EndRenderPass() override {}

        // Draw Binded Buffers
        void DrawIndexed(uint32_t _indexCount) override;

        // Swap Buffers
        void SubmitAndPresent(ISwapChain* _swapchain, ISurface* _surface) override;
    };
}