#pragma once

#include <Resources/resource_interface.h>
#include <matrix4.h>

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
        struct MaterialLightData
        {
            std::unordered_map<std::string, float> m_Floats;
            std::unordered_map<std::string, MirageEngine::Core::Math::Vector3> m_Vec3s;
            std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
        } m_Data;

        friend class ResourceManager;

    private:
        std::shared_ptr<Material> m_Material;
        std::shared_ptr<MirageRHI::IUniformBuffer> m_UniformBuffers;

        std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;

    public:

        MaterialInstance(std::shared_ptr<Material> _material);
        ~MaterialInstance();

        void Bind();

        static std::shared_ptr<MaterialInstance> const GetFromManager(std::string _name);

        std::shared_ptr<Material> GetMaterial() const { return m_Material; }

        void SetTexture(const std::string& _name, std::shared_ptr<Texture> _texture);
        void SetFloat(const std::string& _name, float _value);
        void SetVec3(const std::string& _name, const MirageEngine::Core::Math::Vector3& _value);

        float GetFloat(const std::string& _name, float _defaultValue = 0.0f) const;
        MirageEngine::Core::Math::Vector3 GetVec3(const std::string& _name, const MirageEngine::Core::Math::Vector3& _defaultValue = MirageEngine::Core::Math::Vector3(0.f)) const;
        std::shared_ptr<Texture> GetTexture(const std::string& _name) const;
    };
} // namespace MirageEngine::Core::Resources
