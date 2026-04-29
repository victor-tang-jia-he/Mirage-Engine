#pragma once
#include <unordered_map>
#include <string>
#include <Resources/resource_interface.h>
#include <memory>

namespace MirageRHI
{
    class IUniformBuffer;
    class ITexture;
} // namespace MirageRHI

namespace MirageEngine::Core::Resources
{
    class ResourceManager;
    class Material;
    class Texture;
    class IResource;

    class MaterialInstance : public IResource
    {
            friend class ResourceManager;

            std::shared_ptr<Material>                    m_Material;
            std::shared_ptr<MirageRHI::IUniformBuffer> m_UniformBuffers;

            std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;

        public:

            MaterialInstance(std::shared_ptr<Material> _material, std::shared_ptr<MirageRHI::IUniformBuffer> _ubos);
            ~MaterialInstance();

            void Bind();

            static std::shared_ptr<MaterialInstance> const GetFromManager(std::string _name);

            std::shared_ptr<Material> GetMaterial() const { return m_Material; }

            void SetTexture(const std::string& _name, std::shared_ptr<Texture> _texture);
    };
} // namespace MirageEngine::Core::Resources
