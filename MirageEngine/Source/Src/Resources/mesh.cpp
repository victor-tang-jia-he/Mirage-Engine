#include <Resources/mesh.h>
#include <i_buffer.h>
#include <i_vertex_layout.h>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogMesh)

using namespace MirageEngine::Core::Resources;

Mesh::Mesh(std::shared_ptr<MirageRHI::IVertexLayout> _vao, std::shared_ptr<IBuffer> _vb, std::shared_ptr<IBuffer> _ib, std::vector<LitVertex>&& _vertices, std::vector<uint32_t>&& _indices, size_t _count)
    : m_VertexLayout(_vao)
    , m_VertexBuffer(_vb)
    , m_IndexBuffer(_ib)
    , m_Vertex(std::move(_vertices))
    , m_Index(std::move(_indices))
    , m_IndexCount(_count)
{
    if (m_VertexLayout)
    {
        CreateLitVertexLayout();
    }
}

Mesh::~Mesh()
{
    m_IndexCount = 0;
    m_VertexBuffer = nullptr;
    m_IndexBuffer  = nullptr;
    LOG(logTemp, LogMesh, "Mesh ", p_Name, " destroyed !");
}

void Mesh::Bind()
{
    if (!m_VertexBuffer || !m_IndexBuffer || !m_VertexLayout)
    {
        LOG(logError, LogMesh, "Mesh ", p_Name, " has invalid buffers !");
        return;
    }
    if (!m_bIsUpload)
    {
        m_bIsUpload = true;
        m_VertexBuffer->Create(m_Vertex.data(), m_Vertex.size() * sizeof(LitVertex), MirageRHI::IBuffer::BufferType::Vertices);
        m_IndexBuffer->Create(m_Index.data(), m_Index.size() * sizeof(uint32_t), MirageRHI::IBuffer::BufferType::Indices);
        m_VertexLayout->Create();
        // Clear CPU-side data after uploading to GPU
        m_Vertex.clear();
        m_Index.clear();
    }
}

void MirageEngine::Core::Resources::Mesh::CreateLitVertexLayout()
{
    std::shared_ptr<MirageRHI::VertexAttribute> positionAttr = std::make_shared<MirageRHI::VertexAttribute>();
    positionAttr->Name = "position";
    positionAttr->Size = 3;
    positionAttr->Type = MirageRHI::AttributeType::Float;
    positionAttr->Normalized = false;
    positionAttr->Stride = sizeof(LitVertex);
    positionAttr->Pointer = (void*)offsetof(LitVertex, Position);

    std::shared_ptr<MirageRHI::VertexAttribute> normalAttr = std::make_shared<MirageRHI::VertexAttribute>();
    normalAttr->Name = "normal";
    normalAttr->Size = 3;
    normalAttr->Type = MirageRHI::AttributeType::Float;
    normalAttr->Normalized = false;
    normalAttr->Stride = sizeof(LitVertex);
    normalAttr->Pointer = (void*)offsetof(LitVertex, Normal);

    std::shared_ptr<MirageRHI::VertexAttribute> UVAttr = std::make_shared<MirageRHI::VertexAttribute>();
    UVAttr->Name = "UV";
    UVAttr->Size = 2;
    UVAttr->Type = MirageRHI::AttributeType::Float;
    UVAttr->Normalized = false;
    UVAttr->Stride = sizeof(LitVertex);
    UVAttr->Pointer = (void*)offsetof(LitVertex, UV);

    m_VertexLayout->AddAttribute(positionAttr, 0);
    m_VertexLayout->AddAttribute(normalAttr, 1);
    m_VertexLayout->AddAttribute(UVAttr, 2);
}

void MirageEngine::Core::Resources::Mesh::ClearVertexLayout()
{
    m_VertexLayout->ClearAttributes();
}
