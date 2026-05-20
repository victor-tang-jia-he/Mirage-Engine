#include <Glad/glad.h>
#include <GLFW/glfw3.h>

#include <OpenGL/openGL_framebuffer.h>
#include <OpenGL/openGL_buffer.h>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogRHIFramebuffer)

void MirageRHI::OpenGLFramebuffer::Create(uint32_t _width, uint32_t _height)
{
    // Size
    m_Width = _width;
    m_Height = _height;

    // FrameBuffer
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    // Texture
    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

    // RenderBuffer
    glGenRenderbuffers(1, &m_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

    // Verification
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG(logError, LogRHIFramebuffer, "Framebuffer is not complete!");
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    LOG(logTemp, LogRHIFramebuffer, "Framebuffer Created with ID: ", m_FBO);
}

void MirageRHI::OpenGLFramebuffer::Destroy()
{
    glDeleteFramebuffers(1, &m_FBO);
    glDeleteTextures(1, &m_Texture);
    glDeleteRenderbuffers(1, &m_RBO);

    LOG(logTemp, LogRHIFramebuffer, "Framebuffer Destroyed with ID: ", m_FBO);
}

void MirageRHI::OpenGLFramebuffer::RescaleFrameBuffer(uint32_t _width, uint32_t _height) const
{
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
}

void MirageRHI::OpenGLFramebuffer::Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_FBO); }

void MirageRHI::OpenGLFramebuffer::Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void MirageRHI::OpenGLFramebuffer::BindFrameTexture() const
{
    glActiveTexture(m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}
