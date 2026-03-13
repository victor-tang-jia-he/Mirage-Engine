#include "openGL/openGL_instance.h"

void MirageRHI::OpenGLInstance::Create(std::string _name)
{
	name = _name;
    version = (const char*)glGetString(GL_VERSION);
    renderer = (const char*)glGetString(GL_RENDERER);
    vendor = (const char*)glGetString(GL_VENDOR);

    if (version) {
        DEBUG_LOG("OpenGL Version: [%c].\n", version);
    }
    else {
        DEBUG_THROW("Failed to get OpenGL Version!\n");
    }

    if (renderer) {
        DEBUG_LOG("OpenGL Renderer: [%c].\n", renderer);
    }
    else {
        DEBUG_THROW("Failed to get OpenGL Renderer!\n");
    }

    if (vendor) {
        DEBUG_LOG("OpenGL Vendor: [%c].\n", vendor);
    }
    else {
        DEBUG_THROW("Failed to get OpenGL Vendor!\n");
    }
}
