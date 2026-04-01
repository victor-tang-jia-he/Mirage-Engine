#include <Resources/texture.h>
#include <RHI/i_buffer.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <debug.h>
#define RGBA 0x1908
#define RGB  0x1907

using namespace MirageEngine::Core::Resources;

void Texture::Load(const std::string& _path)
{
    stbi_set_flip_vertically_on_load(true);

    data = stbi_load(_path.c_str(), &width, &height, &channel, 0);
    if (!data) { DEBUG_THROW("Texture Loading Error : file %s is incompatible or doesn't exist !", _path.c_str()); }

    format = (channel == 4) ? RGBA : RGB;
}

void Texture::Bind()
{
    if (m_TextureBuffer == nullptr || data == nullptr)
    {
        // LOG(LogError, LogCat::System, "Texture buffer or data isn't set, use Load to get the texture data before calling bind");
        return;
    }
    if (!m_bIsUploaded)
    {
        m_bIsUploaded = true;
        m_TextureBuffer->Create(static_cast<const void*>(data), size_t(width * height * channel), MirageRHI::IBuffer::BufferType::Texture);
    }
    m_TextureBuffer->Bind();
}
