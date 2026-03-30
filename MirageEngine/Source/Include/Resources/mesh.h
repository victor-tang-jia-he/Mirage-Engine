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
			class IMesh : public IResource
			{
				MirageRHI::IBuffer* m_vertexBuffer = nullptr;
				MirageRHI::IBuffer* m_indicesBuffer = nullptr;
				size_t m_indicesCount = 0;

			public:
				IMesh(MirageRHI::IBuffer* _vertexBuffer, MirageRHI::IBuffer* _indiceBuffer, size_t _indicesCount)
					: m_vertexBuffer(_vertexBuffer), m_indicesBuffer(_indiceBuffer), m_indicesCount(_indicesCount)
				{
				}
				~IMesh() = default;

				MirageRHI::IBuffer* GetVertexBuffer() const { return m_vertexBuffer; }
				MirageRHI::IBuffer* GetIndices() const { return m_indicesBuffer; }
				size_t GetIndicesCount() const { return m_indicesCount; }

			};
		}
	}
}