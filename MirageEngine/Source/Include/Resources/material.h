#pragma once

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

    private:
        std::shared_ptr<MirageRHI::IPipeline> m_Pipeline;
        std::shared_ptr<MirageRHI::IShader> m_VertexShader;
        std::shared_ptr<MirageRHI::IShader> m_FragmentShader;
        size_t m_UniformBufferSize;

    public:
        ~Material() override;

        std::shared_ptr<MirageRHI::IPipeline> GetPipeline() { return m_Pipeline; }
        std::shared_ptr<MirageRHI::IShader> GetVertexShader() { return m_VertexShader; }
        std::shared_ptr<MirageRHI::IShader> GetFragmentShader() { return m_FragmentShader; }
        size_t GetUniformBufferSize() const { return m_UniformBufferSize; }

        static std::shared_ptr<Material> const GetFromManager(std::string _name);
    };
} // namespace MirageEngine::Core::Resources
