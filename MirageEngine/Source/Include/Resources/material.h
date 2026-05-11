#pragma once
#include <vector3.h>

#include <Resources/resource_interface.h>

DECLARE_LOG_CATEGORY_EXTERN(LogMaterial)

namespace MirageRHI
{
    class IPipeline;
    class IShader;
} // namespace MirageRHI

namespace MirageEngine::Core::Resources
{
    class ResourceManager;
    class Mesh;

    class Material : public IResource
    {
            friend class ResourceManager;

            std::shared_ptr<MirageRHI::IPipeline> pipeline;
            std::shared_ptr<MirageRHI::IShader> vertexShader;
            std::shared_ptr<MirageRHI::IShader> fragmentShader;
            size_t uniformBufferSize;

        public:

            ~Material() override;

            std::shared_ptr<MirageRHI::IPipeline> GetPipeline() { return pipeline; }

            std::shared_ptr<MirageRHI::IShader> GetVertexShader() { return vertexShader; }

            std::shared_ptr<MirageRHI::IShader> GetFragmentShader() { return fragmentShader; }

            size_t GetUniformBufferSize() const { return uniformBufferSize; }
    };
} // namespace MirageEngine::Core::Resources
