#pragma once
#include <Resources/resource_interface.h>
#include <vector>
#include <Logs/log_macro.h>
#include <RHI/i_pipeline_parameters.h>

DECLARE_LOG_CATEGORY_EXTERN(LogMesh)

namespace MirageRHI
{
	class IBuffer;
	struct LitVertex;
}

namespace MirageEngine::Core::Resources
{
	class Mesh : public IResource
	{
	private:

		using IBuffer = MirageRHI::IBuffer;
		using LitVertex = MirageRHI::LitVertex;

		std::shared_ptr<IBuffer> m_VertexBuffer = nullptr;
		std::shared_ptr<IBuffer> m_IndexBuffer = nullptr;

		std::vector<uint32_t>  m_Index;
		std::vector<LitVertex> m_Vertex;
		size_t                 m_IndexCount = 0;

		bool m_bIsUpload = false;

	public:

		Mesh(std::shared_ptr<IBuffer> _vb, std::shared_ptr<IBuffer> _ib, std::vector<LitVertex>&& _vertices, std::vector<uint32_t>&& _indices, size_t _count);

		~Mesh() override;

		void Bind();

		std::shared_ptr<MirageRHI::IBuffer> GetVertexBuffer() const { return m_VertexBuffer; }

		std::shared_ptr<MirageRHI::IBuffer> GetIndexBuffer() const { return m_IndexBuffer; }

		size_t GetIndicesCount() const { return m_IndexCount; }
	};
} // namespace MirageEngine
