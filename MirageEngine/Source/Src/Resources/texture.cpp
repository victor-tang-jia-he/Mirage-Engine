#include <Resources/texture.h>
#include <RHI/i_texture.h>
#include <string>

DEFINE_LOG_CATEGORY(LogTexture)

using namespace MirageEngine::Core::Resources;
using namespace MirageRHI;

Texture::~Texture()
{
    if (m_Data != nullptr) free(m_Data);
    if (m_TextureBuffer) m_TextureBuffer->Destroy();
}

void Texture::Bind()
{
    if (m_TextureBuffer == nullptr || m_Data == nullptr)
    {
        LOG(logError, LogTexture, "Texture buffer or data isn't set, use Load to get the texture data before calling bind");
        return;
    }
    if (!m_bIsUploaded)
    {
        m_bIsUploaded = true;
        TextureDesc desc = { static_cast<uint32_t>(m_Width), static_cast<uint32_t>(m_Height), static_cast<uint32_t>(m_Channel), true };
        m_TextureBuffer->Create(static_cast<const void*>(m_Data), desc);
        free(m_Data);
        m_Data = nullptr; // Avoid dangling pointer
    }
    m_TextureBuffer->Bind();
}
