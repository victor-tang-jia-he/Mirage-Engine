#include <OpenGL/openGL_instance.h>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogRHIInstance)

void MirageRHI::OpenGLInstance::Create(std::string _name)
{
    m_Name = _name;
    m_Version = (const char*)glGetString(GL_VERSION);
    m_Renderer = (const char*)glGetString(GL_RENDERER);
    m_Vendor = (const char*)glGetString(GL_VENDOR);

    if (m_Version)
    {
        LOG(logInfo, LogRHIInstance, "OpenGL Version: ", m_Version);
    }
    else
    {
        LOG(logError, LogRHIInstance, "Failed to get OpenGL Version!");
    }

    if (m_Renderer)
    {
        LOG(logInfo, LogRHIInstance, "OpenGL Renderer: ", m_Renderer);
    }
    else
    {
        LOG(logError, LogRHIInstance, "Failed to get OpenGL Renderer!");
    }

    if (m_Vendor)
    {
        LOG(logInfo, LogRHIInstance, "OpenGL Vendor: ", m_Vendor);
    }
    else
    {
        LOG(logError, LogRHIInstance, "Failed to get OpenGL Vendor!");
    }

    LOG(logInfo, LogRHIInstance, "OpenGL Instance '", m_Name, "' created successfully.");
}
