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
			class Texture : public IResource
			{
				MirageRHI::IBuffer* m_textureBuffer = nullptr;

			public:
				Texture(MirageRHI::IBuffer* _textureBuffer)
					: m_textureBuffer(_textureBuffer) {
				};
				~Texture() = default;

				MirageRHI::IBuffer* GetTextureBuffer() const { return m_textureBuffer; }
			};
		}
	}
}