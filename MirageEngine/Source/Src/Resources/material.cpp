#include <Resources/material.h>

#include <RHI/i_texture.h>
#include <RHI/i_shader.h>
#include <Resources/mesh.h>
#include <Resources/texture.h>

DEFINE_LOG_CATEGORY(LogMaterial)

using namespace MirageEngine::Core::Resources;
using namespace MirageRHI;

Material::~Material()
{
    pipeline.reset();
    vertexShader.reset();
    fragmentShader.reset();
    LOG(logTemp, LogMaterial, "Material ", p_Name, " Destroy");
}
