#pragma once

#include "i_instance.h"

namespace MirageRHI
{
    class OpenGLInstance : public IInstance
    {
        std::string m_Name;
        
        const char* m_Version;
        const char* m_Renderer;
        const char* m_Vendor;

    public:
        // Check and store GPU infos
        void Create(std::string _name) override;
        void Destroy() override {}

        OpenGLInstance& API_OpenGL() override { return *this; }
    };
}