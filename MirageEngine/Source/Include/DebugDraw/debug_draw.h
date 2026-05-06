#pragma once
#include <vector>
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
}
namespace MirageEngine
{
    class Renderer;
    namespace Core::Math
    {
        struct Vector3;
        struct Vector4;
    }
}
namespace MirageEngine::DebugDraw
{
    using namespace MirageEngine::Core::Math;
    struct LineVertex
    {
        MirageEngine::Core::Math::Vector3 Pos;
        MirageEngine::Core::Math::Vector4 Color;
    };

    class ENGINE_API DebugDraw
    {
        friend class MirageEngine::Renderer;
    private:
        std::shared_ptr<MirageRHI::IBuffer> m_VertexBufferObject; 
        std::shared_ptr<MirageRHI::IVertexLayout> m_VertexAttributeObject;

        std::shared_ptr<MirageRHI::IPipelineParameters> m_Pipelineparameter;
        std::shared_ptr<MirageRHI::IPipeline> m_Pipeline;
        uint32_t m_MaxLine = 10000;

        std::vector<LineVertex> m_Lines;
    public:

        static DebugDraw& Instance()
        {
            static DebugDraw instance;
            return instance;
        }
        void Init(std::shared_ptr<MirageRHI::IRenderInterface> _rhi);
        void Line(MirageEngine::Core::Math::Vector3 _a, MirageEngine::Core::Math::Vector3 _b, MirageEngine::Core::Math::Vector4 _color);
        void UpdateVertexBuffer();
        void Flush();
        ~DebugDraw();
    };
}
// Helper Macro
#define Debug_Draw MirageEngine::DebugDraw::DebugDraw::Instance()