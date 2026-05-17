#pragma once
#include <Resources/resource_interface.h>


#include <RHI/i_texture.h>

DECLARE_LOG_CATEGORY_EXTERN(LogTexture)

namespace MirageRHI
{
    class ITexture;
}

namespace MirageEngine::Core::Resources
{
    class Texture : public IResource
    {
    private:

        std::shared_ptr<MirageRHI::ITexture> m_TextureBuffer = nullptr;

        int m_Width = 0;
        int m_Height = 0;
        int m_Channel = 0;
        int m_Format = -1;
        unsigned char* m_Data = nullptr;
        bool m_bIsUploaded = false;

    public:
        Texture(std::shared_ptr<MirageRHI::ITexture> _textureBuffer, int& _width, int& _height, int& _channel, int& _format, unsigned char* _data)
            : m_TextureBuffer(_textureBuffer)
            , m_Width(_width)
            , m_Height(_height)
            , m_Channel(_channel)
            , m_Format(_format)
            , m_Data(_data) {};

	    ~Texture();
	    void Bind();
	    uint32_t GetTextureID() const { return m_TextureBuffer->GetID(); }
	    int GetWidth() const { return m_Width; }
	    int GetHeight() const { return m_Height; }

        std::shared_ptr<MirageRHI::ITexture> GetTextureBuffer() const { return m_TextureBuffer; }

	    std::shared_ptr<Texture> const GetFromManager();
	};
} // namespace MirageEngine
