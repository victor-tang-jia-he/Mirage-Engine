#pragma once

#include <i_device.h>

namespace MirageRHI
{
    class OpenGLDevice : public IDevice
    {
    public:
        void Create() override {}
        void Destroy() override {}

        OpenGLDevice& API_OpenGL() override { return *this; }

        void WaitIdle() override {}
    };
}