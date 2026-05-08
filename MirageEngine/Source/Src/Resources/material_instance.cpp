#include <Resources/material_instance.h>
#include <Resources/material.h>
#include <Resources/texture.h>

#include <RHI/i_pipeline.h>
#include <RHI/i_shader.h>
#include <RHI/i_uniform_buffer.h>
#include <Logs/log.h>

namespace MirageEngine::Core::Resources
{
    MaterialInstance::MaterialInstance(std::shared_ptr<Material> _material, std::shared_ptr<MirageRHI::IUniformBuffer> _ubos)
        : m_Material(_material)
        , m_UniformBuffers(std::move(_ubos))
    {
    }

    MaterialInstance::~MaterialInstance()
    {
        m_Material.reset();
        m_UniformBuffers.reset();
        m_Textures.clear();
    }

    void MaterialInstance::SetTexture(const std::string& _name, std::shared_ptr<Texture> _texture)
    {
        if (!_texture)
        {
            LOG(logError, LogMaterial, "Undefine Texture for ", _name);
        }
        m_Data.m_Textures.try_emplace(_name, _texture);
    }

    void MaterialInstance::SetFloat(const std::string& _name, float _value) { m_Data.m_Floats.try_emplace(_name, _value); }

    void MaterialInstance::SetVec3(const std::string& _name, const MirageEngine::Core::Math::Vector3& _value) { m_Data.m_Vec3s[_name] = _value; }

    float MaterialInstance::GetFloat(const std::string& _name, float _defaultValue) const
    {
        auto it = m_Data.m_Floats.find(_name);
        return (it != m_Data.m_Floats.end()) ? it->second : _defaultValue;
    }

    MirageEngine::Core::Math::Vector3 MaterialInstance::GetVec3(const std::string& _name, const MirageEngine::Core::Math::Vector3& _defaultValue) const
    {
        auto it = m_Data.m_Vec3s.find(_name);
        return (it != m_Data.m_Vec3s.end()) ? it->second : _defaultValue;
    }

    std::shared_ptr<Texture> MaterialInstance::GetTexture(const std::string& _name) const
    {
        auto it = m_Data.m_Textures.find(_name);
        return (it != m_Data.m_Textures.end()) ? it->second : nullptr;
    }

    void MaterialInstance::Bind()
    {
        if (std::shared_ptr<MirageRHI::IPipeline> pipeline = m_Material->GetPipeline())
        {
            // Bind textures
            int textureUnit = 0;
            for (const auto& [_name, _texture] : m_Data.m_Textures)
            {
                _texture->Bind();
                pipeline->SetInt(_name, textureUnit);
                textureUnit++;
            }
            // Bind Floats
            for (const auto& [_name, _value] : m_Data.m_Floats)
            {
                pipeline->SetFloat(_name, _value);
            }
            // Bind vec3s
            for (const auto& [_name, _value] : m_Data.m_Vec3s)
            {
                pipeline->SetVec3(_name, _value);
            }
        }
        else
            LOG(logError, LogMaterial, "No shader assigned to material");
    }
} // namespace MirageEngine::Core::Resources
