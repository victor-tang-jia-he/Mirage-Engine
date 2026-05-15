#pragma once
#include <Resources/resource_interface.h>

#include <RHI/i_pipeline_parameters.h>

DECLARE_LOG_CATEGORY_EXTERN(LogMesh)

namespace MirageRHI
{
    class IBuffer;
    class IVertexLayout;
    struct VertexAttribute;
    enum class AttributeType;
} // namespace MirageRHI

namespace MirageEngine::Core::Resources
{
    struct LitVertex
    {
            MirageEngine::Core::Math::Vector3 Position;
            MirageEngine::Core::Math::Vector3 Normal;
            MirageEngine::Core::Math::Vector2 UV;
    };

    class Mesh : public IResource
    {
    private:

        using IBuffer = MirageRHI::IBuffer;

        std::shared_ptr<MirageRHI::IVertexLayout> m_VertexLayout = nullptr;

        std::shared_ptr<IBuffer> m_VertexBuffer = nullptr;
        std::shared_ptr<IBuffer> m_IndexBuffer = nullptr;

        std::vector<uint32_t> m_Index;
        size_t m_IndexCount = 0;

        bool m_bIsUpload = false;

    public:
        std::vector<LitVertex> m_Vertex;

        Mesh(
            std::shared_ptr<MirageRHI::IVertexLayout> _vao, std::shared_ptr<IBuffer> _vb, std::shared_ptr<IBuffer> _ib, std::vector<LitVertex>&& _vertices, std::vector<uint32_t>&& _indices,
            size_t _count);

        ~Mesh() override;

        void Bind();

        std::shared_ptr<MirageRHI::IVertexLayout> GetVertexLayout() const { return m_VertexLayout; }

        std::shared_ptr<MirageRHI::IBuffer> GetVertexBuffer() const { return m_VertexBuffer; }

        static std::shared_ptr<Mesh> const GetFromManager(std::string _name);

        std::shared_ptr<MirageRHI::IBuffer> GetIndexBuffer() const { return m_IndexBuffer; }

        size_t GetIndicesCount() const { return m_IndexCount; }

        void CreateLitVertexLayout();
        void ClearVertexLayout();
    };
} // namespace MirageEngine::Core::Resources
