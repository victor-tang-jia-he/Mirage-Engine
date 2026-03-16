#include "OpenGL/openGL_framebuffer.h"

void MirageRHI::OpenGLFramebuffer::Create()
{
	glGenFramebuffers(1, &m_FBO);
}

void MirageRHI::OpenGLFramebuffer::Destroy()
{
	glDeleteFramebuffers(1, &m_FBO);
}

void MirageRHI::OpenGLFramebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}
