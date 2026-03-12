#include "../../include/openGL/openGL_instance.h"

void MirageRHI::OpenGLInstance::Create(std::string _name)
{
	name = _name;

    std::string version = glGetString(GL_VERSION);
    if (version) {
        DEBUG_LOG("OpenGL Version: %s.\n", version);
    }
    else {
        DEBUG_THROW("Failed to get OpenGL Version!\n");
    }

    std::string renderer = glGetString(GL_RENDERER);
    if (renderer) {
        DEBUG_LOG("OpenGL Renderer: %s.\n", version);
    }
    else {
        DEBUG_THROW("Failed to get OpenGL Renderer!\n");
    }

    std::string vendor = glGetString(GL_VENDOR);
    if (renderer) {
        DEBUG_LOG("OpenGL Vendor: %s.\n", version);
    }
    else {
        DEBUG_THROW("Failed to get OpenGL Vendor!\n");
    }
}
