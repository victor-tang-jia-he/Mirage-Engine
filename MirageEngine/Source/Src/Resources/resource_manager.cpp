#include <Resources/resource_manager.h>
#include <RHI/OpenGL/openGL_buffer.h>
#include <RHI/OpenGL/openGL_pipeline.h>
#include <RHI/OpenGL/openGL_pipeline_parameters.h>
#include <RHI/OpenGL/openGL_shader.h>
#include <RHI/i_texture.h>
#include <RHI/i_render_interface.h>

#include <Resources/mesh.h>
#include <Resources/texture.h>
#include <Resources/material.h>
#include <Resources/shader.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <TinyObjLoader/tiny_obj_loader.h>

 DEFINE_LOG_CATEGORY(LogResourceManager)

using namespace MirageEngine::Core::Resources;

ResourceManager* ResourceManager::m_Instance = nullptr;

ResourceManager* ResourceManager::Instance()
{
    if (m_Instance == nullptr) m_Instance = new ResourceManager();
    return m_Instance;
}

void ResourceManager::Init(MirageRHI::IRenderInterface* _rhi)
{
    m_Rhi = _rhi; // set for later creating resource
}

Texture* ResourceManager::CreateTexture(const std::string& _name, const std::string& _path)
{
    ResourceManager* rm = ResourceManager::Instance();
    if (!rm)
    {
         LOG(logWarning, LogResourceManager, "Resource Manager isn't instanciate or doesn't exist in this context");
        return nullptr;
    }
    if (!rm->m_Rhi)
    {
         LOG(logError, LogResourceManager, "Resource Manager RHI isn't init");
        return nullptr;
    }
    std::string newAssetName = _name;
    auto        it           = rm->m_Resources.find(_name);
    if (it != rm->m_Resources.end()) { newAssetName = rm->GenerateUniqueAssetName(_name); }

    Texture* texture = new Texture(rm->m_Rhi->InstantiateTexture());
    texture->m_Name  = newAssetName;
    texture->Load(_path);
    rm->m_Resources.emplace(newAssetName, texture);
    return texture;
}

Shader* ResourceManager::CreateShader(const std::string& _name, const std::string& _path, const MirageRHI::IShader::ShaderType _type)
{
    ResourceManager* rm = ResourceManager::Instance();
    if (!rm)
    {
         LOG(logWarning, LogResourceManager, "Resource Manager isn't instanciate or doesn't exist in this context");
        return nullptr;
    }
    if (!rm->m_Rhi)
    {
         LOG(logError, LogResourceManager, "Resource Manager RHI isn't init");
        return nullptr;
    }
    std::string newAssetName = _name;
    auto        it           = rm->m_Resources.find(_name);
    if (it != rm->m_Resources.end()) { newAssetName = rm->GenerateUniqueAssetName(_name); }
    MirageRHI::IShader* rhiShader = rm->m_Rhi->InstantiateShader();
    rhiShader->Create(_path.c_str(), _type);

    Shader* shader     = new Shader();
    shader->m_Name     = newAssetName;
    shader->ShaderBind = rhiShader;

    rm->m_Resources.emplace(newAssetName, shader); // global resource
    rm->m_ShaderCache.emplace(_path, rhiShader);   // local cache
    return shader;
}

Mesh* ResourceManager::CreateMesh(const std::string& _name, const std::string& _path)
{
    ResourceManager* rm = ResourceManager::Instance();
    if (!rm)
    {
         LOG(logWarning, LogResourceManager, "Resource Manager isn't instanciate or doesn't exist in this context");
        return nullptr;
    }
    if (!rm->m_Rhi)
    {
         LOG(logError, LogResourceManager, "Resource Manager RHI isn't init");
        return nullptr;
    }
    std::string newAssetName = _name;
    auto        resourceIt   = rm->m_Resources.find(_name);
    if (resourceIt != rm->m_Resources.end()) { newAssetName = rm->GenerateUniqueAssetName(_name); }

    // Load mesh data from file to ram
    std::vector<uint32_t>             indices;
    std::vector<MirageRHI::LitVertex> vertices;

    rm->LoadMeshUtility(_path, vertices, indices);
    Mesh* mesh = new Mesh(rm->m_Rhi->InstantiateBuffer(), rm->m_Rhi->InstantiateBuffer(), vertices, indices, indices.size());

    mesh->m_Name = newAssetName;
    rm->m_Resources.emplace(newAssetName, mesh);
    return mesh;
}

Material* ResourceManager::CreateMaterial(const std::string& _name)
{
    ResourceManager* rm = ResourceManager::Instance();
    if (!rm)
    {
         LOG(logWarning, LogResourceManager, "Resource Manager isn't instanciate or doesn't exist in this context");
        return nullptr;
    }
    std::string newAssetName = _name;
    auto        resourceIt   = rm->m_Resources.find(_name);
    if (resourceIt != rm->m_Resources.end()) { newAssetName = rm->GenerateUniqueAssetName(_name); }
    Material* material = new Material();
    material->m_Name   = newAssetName;
    rm->m_Resources.emplace(newAssetName, material);
    return material;
}

std::string ResourceManager::GenerateUniqueAssetName(const std::string& _baseName)
{
    std::string uniqueName = _baseName;
    int         counter    = 1;
    while (m_Resources.find(uniqueName) != m_Resources.end())
    {
        std::stringstream ss;
        ss << _baseName << "_" << counter;
        uniqueName = ss.str();
        ++counter;
    }
     LOG(logWarning, LogResourceManager, _baseName, " already taken, replaced by ", uniqueName);

    return uniqueName;
}

void ResourceManager::LoadMeshUtility(std::string _path, std::vector<MirageRHI::LitVertex>& _vertices, std::vector<uint32_t>& _indices)
{
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, _path.c_str());

    if (!err.empty()) { std::cerr << "ERROR: " << err << std::endl; }
    if (!ret)
    {
        std::cerr << "Failed to load OBJ: " << _path << std::endl;
        return;
    }

    size_t totalIndices = 0;
    for (const auto& shape : shapes) { totalIndices += shape.mesh.indices.size(); }

    _vertices.reserve(totalIndices);
    _indices.reserve(totalIndices);

    for (const auto& shape : shapes)
    {
        size_t index_offset = 0;

        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
        {
            size_t fv = shape.mesh.num_face_vertices[f];

            std::vector<uint32_t> faceIndices;
            faceIndices.reserve(fv);

            for (size_t v = 0; v < fv; v++)
            {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                MirageRHI::LitVertex vertex;

                // Position
                vertex.Position = MirageEngine::Core::Math::Vector3(attrib.vertices[3 * idx.vertex_index + 0], attrib.vertices[3 * idx.vertex_index + 1], attrib.vertices[3 * idx.vertex_index + 2]);

                // Normal
                if (idx.normal_index >= 0)
                {
                    vertex.Normal = MirageEngine::Core::Math::Vector3(attrib.normals[3 * idx.normal_index + 0], attrib.normals[3 * idx.normal_index + 1], attrib.normals[3 * idx.normal_index + 2]);
                }
                else { vertex.Normal = MirageEngine::Core::Math::Vector3(0, 0, 0); }

                // UV
                if (idx.texcoord_index >= 0) { vertex.UV = MirageEngine::Core::Math::Vector2(attrib.texcoords[2 * idx.texcoord_index + 0], attrib.texcoords[2 * idx.texcoord_index + 1]); }
                else { vertex.UV = MirageEngine::Core::Math::Vector2(0, 0); }

                uint32_t vertexIndex = static_cast<uint32_t>(_vertices.size());
                _vertices.push_back(vertex);

                faceIndices.push_back(vertexIndex);
            }

            // Triangulate faces if necessary
            if (fv == 3)
            {
                _indices.push_back(faceIndices[0]);
                _indices.push_back(faceIndices[1]);
                _indices.push_back(faceIndices[2]);
            }
            else if (fv == 4)
            {
                _indices.push_back(faceIndices[0]);
                _indices.push_back(faceIndices[1]);
                _indices.push_back(faceIndices[2]);

                _indices.push_back(faceIndices[0]);
                _indices.push_back(faceIndices[2]);
                _indices.push_back(faceIndices[3]);
            }
            else
            {
                for (size_t v = 1; v < fv - 1; v++)
                {
                    _indices.push_back(faceIndices[0]);
                    _indices.push_back(faceIndices[v]);
                    _indices.push_back(faceIndices[v + 1]);
                }
            }

            index_offset += fv;
        }
    }
}

void MirageEngine::Core::Resources::ResourceManager::LoadTextureUtility(const std::string& _path)
{
}

MirageEngine::Core::Resources::ResourceManager::~ResourceManager()
{
    for (auto& resource : m_Resources) resource.second.reset();
    for (auto& shader : m_ShaderCache) shader.second.reset();
    m_Resources.clear();
    m_ShaderCache.clear();
}
