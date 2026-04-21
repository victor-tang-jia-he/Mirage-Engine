#include <Resources/mesh.h>
#include <i_buffer.h>

DEFINE_LOG_CATEGORY(LogMesh)

using namespace MirageEngine::Core::Resources;

Mesh::Mesh(std::shared_ptr<IBuffer> _vb, std::shared_ptr<IBuffer> _ib, std::vector<LitVertex>&& _vertices, std::vector<uint32_t>&& _indices, size_t _count)
    : m_VertexBuffer(_vb)
    , m_IndexBuffer(_ib)
    , m_Vertex(std::move(_vertices))
    , m_Index(std::move(_indices))
    , m_IndexCount(_count)
{
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
    if (!m_VertexBuffer || !m_IndexBuffer)
    {
        LOG(logError, LogMesh, "Mesh ", p_Name, " has invalid buffers !");
        return;
    }
    if (!m_bIsUpload)
    {
        m_bIsUpload = true;
        m_VertexBuffer->Create(m_Vertex.data(), m_Vertex.size() * sizeof(MirageRHI::LitVertex), MirageRHI::IBuffer::BufferType::Vertices);
        m_IndexBuffer->Create(m_Index.data(), m_Index.size() * sizeof(uint32_t), MirageRHI::IBuffer::BufferType::Indices);
        // Clear CPU-side data after uploading to GPU
        m_Vertex.clear();
        m_Index.clear();
    }
}
