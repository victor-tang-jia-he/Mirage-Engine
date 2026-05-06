#pragma once

#include <i_instance.h>

#include <Glad/glad.h>

namespace MirageRHI
{
    class OpenGLInstance : public IInstance
    {
            std::string m_Name = "";

            const char* m_Version = nullptr;
            const char* m_Renderer = nullptr;
            const char* m_Vendor = nullptr;

        public:

            // Check and store GPU infos
            void Create(std::string _name) override;

            void Destroy() override {}

            OpenGLInstance& API_OpenGL() override { return *this; }
    };
} // namespace MirageRHI
