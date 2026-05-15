#include <Resources/material.h>

#include <RHI/i_texture.h>
#include <RHI/i_shader.h>
#include <Resources/mesh.h>
#include <Resources/texture.h>
#include <Resources/resource_manager.h>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogMaterial)

using namespace MirageEngine::Core::Resources;
using namespace MirageRHI;

Material::~Material()
{
    m_Pipeline.reset();
    m_VertexShader.reset();
    m_FragmentShader.reset();
    LOG(logTemp, LogMaterial, "Material ", p_Name, " Destroy");
}

std::shared_ptr<Material> const MirageEngine::Core::Resources::Material::GetFromManager(std::string _name)
{
    return ResourceManager::Instance().GetMaterial(_name);
}
