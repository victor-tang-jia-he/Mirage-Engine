#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "i_device.h"

namespace MirageRHI
{
    class OpenGLDevice : public IDevice
    {
    public:
        // TODO : Add more ?
        void Create() override {}
        void Destroy() override {}

        OpenGLDevice& API_OpenGL() override { return *this; }

        void WaitIdle() override {}

        GLuint CreateBuffer();
        GLuint CreateTexture();
    };
}