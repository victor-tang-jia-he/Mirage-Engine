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
			private:
				MirageRHI::IBuffer* m_TextureBuffer = nullptr;

			public:
				Texture(MirageRHI::IBuffer* _textureBuffer)
					: m_TextureBuffer(_textureBuffer) {
				};
				~Texture() = default;

				MirageRHI::IBuffer* GetTextureBuffer() const { return m_TextureBuffer; }
			};
		}
	}
}