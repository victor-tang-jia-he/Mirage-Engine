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

    data = stbi_load(_path.c_str(), &width, &height, &channel, 0);
    if (!data)
    {
        LOG(logError, LogTexture, "Failed to load texture from path: ", _path);
        return;
    }

    format = (channel == 4) ? OPENGL_RGBA : OPENGL_RGB;
}

void Texture::Bind()
{
    if (m_TextureBuffer == nullptr || data == nullptr)
    {
        LOG(logError, LogTexture, "Texture buffer or data isn't set, use Load to get the texture data before calling bind");
        return;
    }
    if (!m_bIsUploaded)
    {
        m_bIsUploaded = true;
        TextureDesc desc = { static_cast<uint32_t>(width), static_cast<uint32_t>(height), static_cast<uint32_t>(channel), true };
        m_TextureBuffer->Create(static_cast<const void*>(data), desc);
        free(data);
        data = nullptr; // Avoid dangling pointer
    }
    m_TextureBuffer->Bind();
}
