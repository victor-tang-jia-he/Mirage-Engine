#pragma once

#include "..\i_instance.h"

namespace MirageRHI
{
    class OpenGLInstance : public IInstance
    {
        std::string name;

    public:
        void Create(std::string _name) override;
        void Destroy() override {}

        OpenGLInstance& API_OpenGL() override { return *this; }
    };
}