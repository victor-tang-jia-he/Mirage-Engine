#pragma once

#include <Scripting/engine_script_api.h>
#include <vector4.h>

// Forward Declaration
namespace MirageRHI
{
    class IRenderInterface;
    class IBuffer;
    class IPipeline;
    class IPipelineParameters;
    class IVertexLayout;
    class ICommandBuffer;
    class IUniformBuffer;
} // namespace MirageRHI

namespace MirageEngine
{
    class Renderer;

    namespace Core::Math
    {
        struct Vector3;
        struct Vector4;
    } // namespace Core::Math
} // namespace MirageEngine

namespace MirageEngine::DebugDraw
{
    struct LineVertex
    {
            MirageEngine::Core::Math::Vector3 Pos;
            MirageEngine::Core::Math::Vector4 Color;
    };

    class ENGINE_API DebugDraw
    {
    private:
        std::shared_ptr<MirageRHI::IBuffer> m_VertexBufferObject;
        std::shared_ptr<MirageRHI::IVertexLayout> m_VertexAttributeObject;

        std::shared_ptr<MirageRHI::IPipelineParameters> m_Pipelineparameter;
        std::shared_ptr<MirageRHI::IPipeline> m_Pipeline;
        std::shared_ptr<MirageRHI::IUniformBuffer> m_DebugUBO = nullptr;
        uint32_t m_MaxLine = 10000;

        std::vector<LineVertex> m_Lines;

    public:
        static DebugDraw& Instance()
        {
            static DebugDraw instance;
            return instance;
        }

        void Init(std::shared_ptr<MirageRHI::IRenderInterface> _rhi);
        void ShutDown();
        void Line(MirageEngine::Core::Math::Vector3 _a, MirageEngine::Core::Math::Vector3 _b, MirageEngine::Core::Math::Vector4 _color);
        void UpdateVertexBuffer();
        void UpdateUBO(const void* data, size_t size);
        void Flush(std::shared_ptr<MirageRHI::ICommandBuffer> _commandBuffer);
        ~DebugDraw();
    };
} // namespace MirageEngine::DebugDraw

// Helper Macro
#define Debug_Draw MirageEngine::DebugDraw::DebugDraw::Instance()
