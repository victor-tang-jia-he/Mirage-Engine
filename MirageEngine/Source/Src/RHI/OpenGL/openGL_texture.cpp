#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <OpenGL/openGL_texture.h>

#include <Logs/log_macro.h>

DEFINE_LOG_CATEGORY(RHITexture)

using namespace MirageRHI;

void MirageRHI::OpenGLTexture::Create(const void* _data, TextureDesc _desc)
{
    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (_desc.channel == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, _desc.width, _desc.height, 0, format, GL_UNSIGNED_BYTE, _data);

	if (_desc.mipmaps)
        glGenerateMipmap(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

	LOG(logInfo, RHITexture, "Texture Created with ID: ", m_Texture, " (", _desc.width, "x", _desc.height, ", Channels: ", _desc.channel, ", Mipmaps: ", _desc.mipmaps ? "Yes" : "No", ")");
}

void MirageRHI::OpenGLTexture::Destroy()
{
    glDeleteTextures(1, &m_Texture);

	LOG(logInfo, RHITexture, "Texture Destroyed with ID: ", m_Texture);
}

void MirageRHI::OpenGLTexture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}
