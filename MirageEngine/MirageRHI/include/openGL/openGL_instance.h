#pragma once

#include "..\i_instance.h"

namespace MirageRHI
{
    class OpenGLInstance : public IInstance
    {
    public:
        void Create() override;
        void Destroy() override;

        OpenGLInstance& API_OpenGL() override { return *this; }
    };
}