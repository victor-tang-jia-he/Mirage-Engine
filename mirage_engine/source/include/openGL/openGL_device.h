#pragma once

#include "i_device.h"

namespace MirageRHI
{
    class OpenGLDevice : public IDevice
    {
    public:
        // TODO : Add more ?
        void Create() override {}
        void Destroy() override {}

        void WaitIdle() override {}

        GLuint CreateBuffer();
        GLuint CreateTexture();
    };
}