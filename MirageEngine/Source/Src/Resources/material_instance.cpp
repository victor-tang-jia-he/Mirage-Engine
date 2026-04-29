#include <Resources/material_instance.h>
#include <Resources/material.h>
#include <Resources/texture.h>
#include <Resources/resource_manager.h>

#include <RHI/i_pipeline.h>
#include <RHI/i_shader.h>
#include <RHI/i_uniform_buffer.h>

MirageEngine::Core::Resources::MaterialInstance::MaterialInstance(std::shared_ptr<Material> _material, std::shared_ptr<MirageRHI::IUniformBuffer> _ubos)
    : m_Material(_material)
    , m_UniformBuffers(std::move(_ubos))
{
}

MirageEngine::Core::Resources::MaterialInstance::~MaterialInstance()
{
    m_Material.reset();
    m_UniformBuffers.reset();
    m_Textures.clear();
}

void MirageEngine::Core::Resources::MaterialInstance::Bind()
{
    std::shared_ptr<MirageRHI::IPipeline> pipeline = m_Material->GetPipeline();
    int textureUnit = 0;
    for (const auto& [_name, _texture] : m_Textures)
    {
        _texture->Bind();
        pipeline->SetInt(_name, textureUnit);
        textureUnit++;
    }
}

std::shared_ptr<MirageEngine::Core::Resources::MaterialInstance> const MirageEngine::Core::Resources::MaterialInstance::GetFromManager(std::string _name)
{
    return MirageEngine::Core::Resources::ResourceManager::Instance().GetMaterialInstance(_name);
}

void MirageEngine::Core::Resources::MaterialInstance::SetTexture(const std::string& _name, std::shared_ptr<Texture> _texture)
{
    if (!_texture) { LOG(logError, LogMaterial, "Undefine Texture for ", _name); }
    m_Textures.try_emplace(_name, _texture);
}
