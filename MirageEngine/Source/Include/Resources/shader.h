#pragma once
#include <Resources/resource_interface.h>
#include <RHI/i_shader.h>

namespace MirageRHI
{
    class IShader;
}

namespace MirageEngine::Core::Resources
{
    struct Shader : public IResource
    {
        public:

            std::shared_ptr<MirageRHI::IShader> ShaderBind = nullptr;

            Shader() = default;

            ~Shader() { ShaderBind.reset(); }
    };
} // namespace MirageEngine::Core::Resources
