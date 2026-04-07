#include "OpenGL/openGL_instance.h"

#include <Logs/log_macro.h>

DEFINE_LOG_CATEGORY(RHIInstance)

void MirageRHI::OpenGLInstance::Create(std::string _name)
{
	m_Name = _name;
    m_Version = (const char*)glGetString(GL_VERSION);
    m_Renderer = (const char*)glGetString(GL_RENDERER);
    m_Vendor = (const char*)glGetString(GL_VENDOR);

    if (m_Version) {
        LOG(logInfo, RHIInstance, "OpenGL Version: ", m_Version);
    }
    else {
        LOG(logError, RHIInstance, "Failed to get OpenGL Version!");
    }

    if (m_Renderer) {
        LOG(logInfo, RHIInstance, "OpenGL Renderer: ", m_Renderer);
    }
    else {
        LOG(logError, RHIInstance, "Failed to get OpenGL Renderer!");
    }

    if (m_Vendor) {
        LOG(logInfo, RHIInstance, "OpenGL Vendor: ", m_Vendor);
    }
    else {
        LOG(logError, RHIInstance, "Failed to get OpenGL Vendor!");
    }

	LOG(logInfo, RHIInstance, "OpenGL Instance '", m_Name, "' created successfully.");
}
