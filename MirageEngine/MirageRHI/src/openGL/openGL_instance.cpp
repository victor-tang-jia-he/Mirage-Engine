#include "../../include/openGL/openGL_instance.h"

void MirageRHI::OpenGLInstance::Create(std::string _name)
{
	name = _name;
    version = glGetString(GL_VERSION);
    renderer = glGetString(GL_RENDERER);
    vendor = glGetString(GL_VENDOR);

    if (version) {
        DEBUG_LOG("OpenGL Version: %s.\n", version);
    }
    else {
        DEBUG_THROW("Failed to get OpenGL Version!\n");
    }

    if (renderer) {
        DEBUG_LOG("OpenGL Renderer: %s.\n", renderer);
    }
    else {
        DEBUG_THROW("Failed to get OpenGL Renderer!\n");
    }

    if (vendor) {
        DEBUG_LOG("OpenGL Vendor: %s.\n", vendor);
    }
    else {
        DEBUG_THROW("Failed to get OpenGL Vendor!\n");
    }
}
