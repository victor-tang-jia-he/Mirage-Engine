#pragma once

#include "..\i_instance.h"

namespace MirageRHI
{
    class OpenGLInstance : public IInstance
    {
        std::string name;
        
        const std::string version;
        const std::string renderer;
        const std::string vendor;

    public:
        // Check and store GPU infos
        void Create(std::string _name) override;
        void Destroy() override {}

        OpenGLInstance& API_OpenGL() override { return *this; }
    };
}