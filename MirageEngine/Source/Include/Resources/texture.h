#pragma once
#include <Resources/resource_interface.h>
#include <Logs/log_macro.h>

DECLARE_LOG_CATEGORY_EXTERN(LogTexture)

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

                    int            width   = 0;
                    int            height  = 0;
                    int            channel = 0;
                    int            format  = NAN;
                    unsigned char* data    = nullptr;
                    bool           m_bIsUploaded = false;

                public:

                    Texture(MirageRHI::IBuffer* _textureBuffer)
                        : m_TextureBuffer(_textureBuffer) {};

                    ~Texture()
                    {
                        if (data != nullptr) free(data);
                    }

                    void Load(const std::string& _path);

                    void Bind();

                    MirageRHI::IBuffer* GetTextureBuffer() const { return m_TextureBuffer; }
            };
        } // namespace Resources
    } // namespace Core
} // namespace MirageEngine
