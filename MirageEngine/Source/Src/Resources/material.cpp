#include <Resources/material.h>

#include <RHI/i_texture.h>
#include <RHI/i_shader.h>
#include <Resources/mesh.h>
#include <Resources/texture.h>


DEFINE_LOG_CATEGORY(LogMaterial)

using namespace MirageEngine::Core::Resources;
using namespace MirageRHI;

Material::~Material() { m_Shader.reset(); }

void Material::SetShader(std::shared_ptr<MirageRHI::IPipeline> _shader)
{
    if (!_shader) { LOG(logError, LogMaterial, "Undefine Shader"); }
    this->m_Shader = _shader;
}

Material& Material::SetFloat(const std::string& _name, float _value)
{
    m_Data.Floats[_name] = _value;
    return *this;
}

Material& Material::SetVec3(const std::string& _name, const MirageEngine::Core::Math::Vector3& _value)
{
    m_Data.Vec3s[_name] = _value;
    return *this;
}

Material& Material::SetColor(const std::string& _name, const MirageEngine::Core::Math::Vector3& _color) { return SetVec3(_name, _color); }

Material& Material::SetTexture(const std::string& _name, std::shared_ptr<Texture> _texture)
{
    if (!_texture) { LOG(logError, LogMaterial, "Undefine Texture for ", _name.c_str()); }
    m_Data.Textures[_name] = _texture;
    return *this;
}

float Material::GetFloat(const std::string& _name, float _defaultValue) const
{
    auto it = m_Data.Floats.find(_name);
    return (it != m_Data.Floats.end()) ? it->second : _defaultValue;
}

MirageEngine::Core::Math::Vector3 Material::GetVec3(const std::string& _name, const MirageEngine::Core::Math::Vector3& _defaultValue) const
{
    auto it = m_Data.Vec3s.find(_name);
    return (it != m_Data.Vec3s.end()) ? it->second : _defaultValue;
}

std::shared_ptr<Texture> Material::GetTexture(const std::string& _name) const
{
    auto it = m_Data.Textures.find(_name);
    return (it != m_Data.Textures.end()) ? it->second.lock() : nullptr;
}

void Material::Bind()
{
    if (!m_Shader)
    {
        LOG(logError, LogMaterial, "No shader assigned to material");
        return;
    }

    // Bind textures
    int textureUnit = 0;
    for (const auto& [_name, _texture] : m_Data.Textures)
    {
        if (!_texture.expired())
        {
            _texture.lock()->Bind();
            m_Shader->SetInt(_name, textureUnit);
            textureUnit++;
        }
    }
    // Bind Floats
    for (const auto& [_name, _value] : m_Data.Floats) { m_Shader->SetFloat(_name, _value); }
    // Bind vec3s
    for (const auto& [_name, _value] : m_Data.Vec3s) { m_Shader->SetVec3(_name, _value); }
}
