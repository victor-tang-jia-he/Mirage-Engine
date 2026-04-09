#include <Resources/texture.h>
#include <RHI/i_texture.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <debug.h>
constexpr auto OPENGL_RGBA = 0x1908;
constexpr auto OPENGL_RGB = 0x1907;

DEFINE_LOG_CATEGORY(LogTexture)

using namespace MirageEngine::Core::Resources;
using namespace MirageRHI;

void Texture::Load(const std::string& _path)
{
    stbi_set_flip_vertically_on_load(true);

    m_Data = stbi_load(_path.c_str(), &m_Width, &m_Height, &m_Channel, 0);
    if (!m_Data)
    {
        LOG(logError, LogTexture, "Failed to load texture from path: ", _path);
        return;
    }

    m_Format = (m_Channel == 4) ? OPENGL_RGBA : OPENGL_RGB;
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
