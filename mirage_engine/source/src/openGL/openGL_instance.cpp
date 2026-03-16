#include "openGL/openGL_instance.h"

void MirageRHI::OpenGLInstance::Create(std::string _name)
{
	m_Name = _name;
    m_Version = (const char*)glGetString(GL_VERSION);
    m_Renderer = (const char*)glGetString(GL_RENDERER);
    m_Vendor = (const char*)glGetString(GL_VENDOR);

    if (m_Version) {
        DEBUG_LOG("OpenGL Version: [%c].\n", m_Version);
    }
    else {
        DEBUG_THROW("Failed to get OpenGL Version!\n");
    }

    if (m_Renderer) {
        DEBUG_LOG("OpenGL Renderer: [%c].\n", m_Renderer);
    }
    else {
        DEBUG_THROW("Failed to get OpenGL Renderer!\n");
    }

    if (m_Vendor) {
        DEBUG_LOG("OpenGL Vendor: [%c].\n", m_Vendor);
    }
    else {
        DEBUG_THROW("Failed to get OpenGL Vendor!\n");
    }
}
