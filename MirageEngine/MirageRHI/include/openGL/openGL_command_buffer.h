#pragma once

#include "..\i_command_buffer.h"

namespace MirageRHI
{
    class OpenGLCommandBuffer : public ICommandBuffer
    {
    public:
        void Create() override {}
        void Destroy() override {}

        OpenGLCommandBuffer& API_OpenGL() override { return *this; }

        void BeginDraw() override {}
        void EndDraw() override {}

        void BindPipeline(IPipeline* pipeline) override;
        void BindVertexBuffer(IBuffer* buffer) override;
        void BindIndexBuffer(IBuffer* buffer) override;

        void DrawIndexed(uint32_t indexCount) override;
    };
}