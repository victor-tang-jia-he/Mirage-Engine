#include <Resources/mesh.h>
#include <i_buffer.h>
#include <i_pipeline_parameters.h>

DEFINE_LOG_CATEGORY(LogMesh)

using namespace MirageEngine::Core::Resources;

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

    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();
}
