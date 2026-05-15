// debug_draw.cpp
#include <DebugDraw/debug_draw.h>
#include <stdexcept>
#include <cstddef> // offsetof

// RHI Include
#include <RHI/i_render_interface.h>
#include <RHI/i_shader.h>
#include <RHI/i_pipeline.h>
#include <RHI/i_pipeline_parameters.h>
#include <RHI/i_vertex_layout.h>
#include <RHI/i_buffer.h>
#include <RHI/i_command_buffer.h>

// resource Include
#include <Resources/shader.h>
#include <Resources/resource_manager.h>

#include <matrix4.h>
#include <i_uniform_buffer.h>

namespace MirageEngine::DebugDraw
{
    using namespace MirageEngine::Core::Resources;
    using namespace MirageRHI;

    // ------------------------------------------------------------------ init

    void DebugDraw::Init(std::shared_ptr<MirageRHI::IRenderInterface> _rhi)
    {
        // All Instance
        ResourceManager& rm = ResourceManager::Instance();
        // Compile & link
        // ------------------------------------------------------------------ shaders

        std::shared_ptr<Shader> vert = rm.GetVertexShader("debug_line_Vertex");
        std::shared_ptr<Shader> frag = rm.GetVertexShader("debug_line_Fragment");

        // ------------------------------------------------------------------ VAO

        std::shared_ptr<VertexAttribute> vertexAPosition = std::make_shared<VertexAttribute>();
        vertexAPosition->Name = "aPosition";
        vertexAPosition->Type = AttributeType::FLOAT;
        vertexAPosition->Size = 3;
        vertexAPosition->Stride = sizeof(LineVertex);
        vertexAPosition->Normalized = false;
        vertexAPosition->Pointer = (void*)offsetof(LineVertex, Pos);

        std::shared_ptr<VertexAttribute> vertexAColor = std::make_shared<VertexAttribute>();
        vertexAColor->Name = "aColor";
        vertexAColor->Type = AttributeType::FLOAT;
        vertexAColor->Size = 4;
        vertexAColor->Stride = sizeof(LineVertex);
        vertexAColor->Normalized = false;
        vertexAColor->Pointer = (void*)offsetof(LineVertex, Color);

        m_VertexAttributeObject = _rhi->InstantiateVertexLayout();
        m_VertexAttributeObject->AddAttribute(vertexAPosition, 0);
        m_VertexAttributeObject->AddAttribute(vertexAColor, 1);
        m_VertexAttributeObject->Create(); // Create VAO

        m_VertexBufferObject = _rhi->InstantiateBuffer();
        m_VertexBufferObject->Create(nullptr, sizeof(LineVertex) * m_MaxLine, IBuffer::BufferType::VERTICES_DYNAMIC);

        // ------------------------------------------------------------------ Pipeline
        m_Pipelineparameter = _rhi->InstantiatePipelineParameters();
        m_Pipelineparameter->Create(MirageRHI::CullingType::NONE, false); // Default setting

        m_Pipeline = _rhi->InstantiatePipeline();
        m_Pipeline->Create(
            vert->ShaderBind, frag->ShaderBind, m_Pipelineparameter,
            {
                { "PerFrame", MirageRHI::UBOBinding::PER_FRAME }
            });

        // ------------------------------------------------------------------ Uniform Buffer
        m_DebugUBO = _rhi->InstantiateUniformBuffer();

        m_DebugUBO->Create(MirageRHI::UBOBinding::PER_FRAME, sizeof(MirageRHI::PerFrameData));

    }

    // ------------------------------------------------------------------ Destructor

    void DebugDraw::ShutDown()
    {
        m_VertexAttributeObject->Destroy();
        m_VertexBufferObject->Destroy();
        m_Pipelineparameter->Destroy();
        m_Pipeline->Destroy();
        m_Lines.clear();
    }


    // ------------------------------------------------------------------ API

    void DebugDraw::Line(Vector3 _a, Vector3 _b, Vector4 _color)
    {
        m_Lines.push_back({ _a, _color });
        m_Lines.push_back({ _b, _color });
    }

    void DebugDraw::UpdateVertexBuffer()
    {
        if (m_Lines.empty())
            return;

        m_VertexBufferObject->Update(m_Lines.data(), m_Lines.size() * sizeof(LineVertex));
    }

    void DebugDraw::UpdateUBO(const void* data, size_t size) { m_DebugUBO->Upload(data, size); }

    void DebugDraw::Flush(std::shared_ptr<MirageRHI::ICommandBuffer> _commandBuffer)
    {
        _commandBuffer->BindPipeline(m_Pipeline);
        _commandBuffer->ApplyPipelineParameters(m_Pipeline);
        UpdateVertexBuffer();
        _commandBuffer->BindBuffer(m_VertexBufferObject);
        _commandBuffer->ApplyVertexLayout(m_VertexAttributeObject);
        _commandBuffer->Draw((uint32_t)m_Lines.size(), MirageRHI::DrawMode::LINES);
        m_Lines.clear();
    }

    DebugDraw::~DebugDraw()
    {  }

    // ------------------------------------------------------------------ helpers
} // namespace MirageEngine::DebugDraw
