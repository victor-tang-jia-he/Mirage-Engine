#pragma once

#include "..\i_instance.h"

namespace MirageRHI
{
    class OpenGLInstance : public IInstance
    {
        std::string name;
        
        const char* version;
        const char* renderer;
        const char* vendor;

    public:
        // Check and store GPU infos
        void Create(std::string _name) override;
        void Destroy() override {}

        OpenGLInstance& API_OpenGL() override { return *this; }
    };
}