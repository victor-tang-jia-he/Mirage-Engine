#pragma once
#include <Resources/resource_interface.h>

namespace MirageRHI
{
	class IBuffer;
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
				MirageRHI::IBuffer* m_VertexBuffer = nullptr;
				MirageRHI::IBuffer* m_IndicesBuffer = nullptr;
				size_t m_IndicesCount = 0;

			public:
				Mesh(MirageRHI::IBuffer* _vertexBuffer, MirageRHI::IBuffer* _indiceBuffer, size_t _indicesCount)
					: m_VertexBuffer(_vertexBuffer), m_IndicesBuffer(_indiceBuffer), m_IndicesCount(_indicesCount)
				{
				}
				~Mesh() = default;

				void Bind() const;

				MirageRHI::IBuffer* GetVertexBuffer() const { return m_VertexBuffer; }
				MirageRHI::IBuffer* GetIndexBuffer() const { return m_IndicesBuffer; }
				size_t GetIndicesCount() const { return m_IndicesCount; }

			};
		}
	}
}