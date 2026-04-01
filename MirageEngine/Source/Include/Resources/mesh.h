#pragma once
#include <Resources/resource_interface.h>
#include <vector>

namespace MirageRHI
{
    class IBuffer;
    struct LitVertex;
}

namespace MirageEngine
{
    namespace Core
    {
        namespace Resources
        {
            class Mesh : public IResource
            {
                private:

                    using IBuffer   = MirageRHI::IBuffer;
                    using LitVertex = MirageRHI::LitVertex;

                    IBuffer* m_VertexBuffer = nullptr;
                    IBuffer* m_IndexBuffer  = nullptr;

                    std::vector<uint32_t>  m_Index;
                    std::vector<LitVertex> m_Vertex;
                    size_t                 m_IndexCount = 0;

                    bool m_bIsUpload = false;

                public:

                    Mesh(IBuffer* _vertexBuffer, IBuffer* _indiceBuffer, std::vector<LitVertex> _vertex, std::vector<uint32_t> _indice, size_t _IndexCount)
                        : m_VertexBuffer(_vertexBuffer)
                        , m_IndexBuffer(_indiceBuffer)
                        , m_Vertex(_vertex)
                        , m_Index(_indice)
                        , m_IndexCount(_IndexCount)
                    {
                    }

                    ~Mesh() = default;

                    void Bind();

                    MirageRHI::IBuffer* GetVertexBuffer() const { return m_VertexBuffer; }

                    MirageRHI::IBuffer* GetIndexBuffer() const { return m_IndexBuffer; }

                    size_t GetIndicesCount() const { return m_IndexCount; }
            };
        } // namespace Resources
    } // namespace Core
} // namespace MirageEngine
