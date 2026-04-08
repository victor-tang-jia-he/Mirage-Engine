#pragma once
#include <Resources/resource_interface.h>
#include <Logs/log_macro.h>

DECLARE_LOG_CATEGORY_EXTERN(LogTexture)

namespace MirageRHI
{
    class ITexture;
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

                    MirageRHI::ITexture* m_TextureBuffer = nullptr;

                    int            m_Width   = 0;
                    int            m_Height  = 0;
                    int            m_Channel = 0;
                    int            m_Format  = NAN;
                    unsigned char* m_Data    = nullptr;
                    bool           m_bIsUploaded = false;

                public:

                    Texture(MirageRHI::ITexture* _textureBuffer)
                        : m_TextureBuffer(_textureBuffer) {};

                    ~Texture()
                    {
                        if (m_Data != nullptr) free(m_Data);
                    }

                    void Load(const std::string& _path);

                    void Bind();

                    MirageRHI::ITexture* GetTextureBuffer() const { return m_TextureBuffer; }
            };
        } // namespace Resources
    } // namespace Core
} // namespace MirageEngine
