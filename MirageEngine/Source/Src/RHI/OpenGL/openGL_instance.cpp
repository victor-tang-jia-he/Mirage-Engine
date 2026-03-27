#include "OpenGL/openGL_instance.h"

void MirageRHI::OpenGLInstance::Create(std::string _name)
{
	m_Name = _name;
    m_Version = (const char*)glGetString(GL_VERSION);
    m_Renderer = (const char*)glGetString(GL_RENDERER);
    m_Vendor = (const char*)glGetString(GL_VENDOR);

    if (m_Version) {
        DEBUG_LOG("OpenGL Version: [%s].", m_Version);
    }
    else {
        DEBUG_THROW("Failed to get OpenGL Version!");
    }

    if (m_Renderer) {
        DEBUG_LOG("OpenGL Renderer: [%s].", m_Renderer);
    }
    else {
        DEBUG_THROW("Failed to get OpenGL Renderer!");
    }

    if (m_Vendor) {
        DEBUG_LOG("OpenGL Vendor: [%s].", m_Vendor);
    }
    else {
        DEBUG_THROW("Failed to get OpenGL Vendor!");
    }
}
