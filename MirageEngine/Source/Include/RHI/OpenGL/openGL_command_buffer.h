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
        void BeginDraw(std::shared_ptr<IRenderPass> _renderPass, uint32_t _FBO) override;
        void EndDraw() override {}

        // Binding Functions
        void BindPipeline(std::shared_ptr<IPipeline> _pipeline) override;
        void BindBuffer(std::shared_ptr<IBuffer> _buffer) override;
        void BindTextureFromID(uint32_t _ID) override;

        void ApplyPipelineParameters(std::shared_ptr<IPipeline> _pipeline) override;

        // Draw Binded Buffers
        void Draw(uint32_t _indexCount) override;

        // Swap Buffers
        void SubmitAndPresent(std::shared_ptr<ISwapChain> _swapchain, std::shared_ptr<ISurface> _surface) override;
    };
}