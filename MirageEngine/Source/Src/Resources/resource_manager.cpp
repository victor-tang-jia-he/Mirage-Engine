#include <Resources/resource_manager.h>
#include <RHI/OpenGL/openGL_render_interface.h>

#include <Resources/mesh.h>
#include <Resources/texture.h>
#include <Resources/material_instance.h>
#include <Resources/material.h>
#include <Resources/shader.h>
#include <Resources/pipeline.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <TinyObjLoader/tiny_obj_loader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <Logs/log.h>

constexpr int OPENGL_RGBA = 0x1908;
constexpr int OPENGL_RGB = 0x1907;

#include <filesystem>
namespace fs = std::filesystem;

DEFINE_LOG_CATEGORY(LogResourceManager)

using namespace MirageEngine::Core::Resources;

ResourceManager* ResourceManager::m_Instance = nullptr;

void ResourceManager::Shutdown()
{
    m_Rhi.reset();
    m_Resources.clear();
    m_ShaderCache.clear();
    delete m_Instance;
    m_Instance = nullptr;
}

ResourceManager::~ResourceManager() {}

void ResourceManager::Init(std::shared_ptr<MirageRHI::IRenderInterface> _rhi)
{
    m_Rhi = _rhi; // set for later creating resource
}

void ResourceManager::PreLoadResources()
{
    LOG(logInfo, LogResourceManager, "Preloading default resources...");

    CreateShader("DefaultVertex", "Assets/Defaults/default.vert", MirageRHI::IShader::ShaderType::Vertex);
    CreateShader("DefaultFragment", "Assets/Defaults/default.frag", MirageRHI::IShader::ShaderType::Fragment);
    CreateTexture("DefaultTexture", "Assets/Defaults/default.png")->Bind();

    LOG(logInfo, LogResourceManager, "Preloading default resources completed.");
    LOG(logInfo, LogResourceManager, "Preloading resources...");

    // Default directory for resources
    std::string path = "Assets/";
    for (auto it = fs::recursive_directory_iterator(path); it != fs::recursive_directory_iterator(); ++it)
    {
        if (it->is_directory() && it->path().filename() == "Defaults")
        {
            it.disable_recursion_pending();
            continue;
        }

        const fs::directory_entry& entry = *it;

        std::string extension = entry.path().extension().string();

        if (extension == ".obj")
        {
            CreateMesh(entry.path().stem().string(), entry.path().string());
        }
        else if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
        {
            CreateTexture(entry.path().stem().string(), entry.path().string());
        }
        else if (extension == ".vert")
        {
            std::string shaderName = entry.path().stem().string();
            std::string fragPath = entry.path().parent_path().string() + "/" + shaderName + ".frag";
            if (fs::exists(fragPath))
            {
                CreateShader(shaderName + "_Vertex", entry.path().string(), MirageRHI::IShader::ShaderType::Vertex);
                CreateShader(shaderName + "_Fragment", fragPath, MirageRHI::IShader::ShaderType::Fragment);
            }
            else
            {
                LOG(logWarning, LogResourceManager, "Fragment shader not found for ", shaderName);
            }
        }
    }

    LOG(logInfo, LogResourceManager, "Preloading completed.");
}

std::shared_ptr<Texture> ResourceManager::CreateTexture(const std::string& _name, const std::string& _path)
{
    ResourceManager& rm = ResourceManager::Instance();

    if (!rm.m_Rhi)
    {
        LOG(logError, LogResourceManager, "Resource Manager RHI isn't init");
        return nullptr;
    }
    LOG(logTemp, LogResourceManager, "Loading Texture : ", _path);
    std::string newAssetName = _name;
    auto it = rm.m_Resources.find(_name);
    if (it != rm.m_Resources.end())
    {
        newAssetName = rm.GenerateUniqueAssetName(_name);
    }

    int width = 0;
    int height = 0;
    int channel = 0;
    int format = NAN;
    unsigned char* data = nullptr;
    rm.LoadTextureUtility(_path, width, height, channel, format, data);

    auto texture = std::make_shared<Texture>(rm.m_Rhi->InstantiateTexture(), width, height, channel, format, data);

    texture->p_Name = newAssetName;
    texture->p_Path = _path;
    rm.m_Resources.emplace(newAssetName, texture);
    LOG(logTemp, LogResourceManager, "Finish loading Texture : ", _path);
    return texture;
}

std::shared_ptr<Shader> ResourceManager::CreateShader(const std::string& _name, const std::string& _path, const MirageRHI::IShader::ShaderType _type)
{
    ResourceManager& rm = ResourceManager::Instance();

    if (!rm.m_Rhi)
    {
        LOG(logError, LogResourceManager, "Resource Manager RHI isn't init");
        return nullptr;
    }
    auto shaderCacheIt = rm.m_ShaderCache.find(_path);

    if (shaderCacheIt != rm.m_ShaderCache.end())
    {
        LOG(logTemp, LogResourceManager, "Shader already loaded in cache : ", _path);
        return shaderCacheIt->second;
    }

    LOG(logTemp, LogResourceManager, "Loading Shader : ", _path);
    std::string newAssetName = _name;
    auto it = rm.m_Resources.find(_name);
    if (it != rm.m_Resources.end())
    {
        newAssetName = rm.GenerateUniqueAssetName(_name);
    }
    std::shared_ptr<MirageRHI::IShader> rhiShader = rm.m_Rhi->InstantiateShader();
    if (!rhiShader->Create(_path.c_str(), _type))
    {
        rhiShader->Destroy();
        return _type == MirageRHI::IShader::ShaderType::Fragment ? rm.GetFragmentShader("default") : rm.GetVertexShader("default");
    }

    std::shared_ptr<Shader> shader = std::make_shared<Shader>();
    shader->p_Name = newAssetName;
    shader->p_Path = _path;
    shader->ShaderBind = rhiShader;

    rm.m_Resources.emplace(newAssetName, shader);
    rm.m_ShaderCache.emplace(_path, shader);

    LOG(logTemp, LogResourceManager, "Finish loading Shader : ", _path);
    return shader;
}

std::shared_ptr<Mesh> ResourceManager::CreateMesh(const std::string& _name, const std::string& _path)
{
    ResourceManager& rm = ResourceManager::Instance();

    if (!rm.m_Rhi)
    {
        LOG(logError, LogResourceManager, "Resource Manager RHI isn't init");
        return nullptr;
    }
    LOG(logTemp, LogResourceManager, "Loading Mesh : ", _path);
    std::string newAssetName = _name;
    auto resourceIt = rm.m_Resources.find(_name);
    if (resourceIt != rm.m_Resources.end())
    {
        newAssetName = rm.GenerateUniqueAssetName(_name);
    }

    // Load mesh data from file to ram
    std::vector<uint32_t> indices;
    std::vector<LitVertex> vertices;

    rm.LoadMeshUtility(_path, vertices, indices);
    std::shared_ptr<Mesh> mesh
        = std::make_shared<Mesh>(rm.m_Rhi->InstantiateVertexLayout(), rm.m_Rhi->InstantiateBuffer(), rm.m_Rhi->InstantiateBuffer(), std::move(vertices), std::move(indices), indices.size());

    mesh->p_Name = newAssetName;
    mesh->p_Path = _path;
    rm.m_Resources.emplace(newAssetName, mesh);
    LOG(logTemp, LogResourceManager, "Finish loading Mesh : ", _path);
    return mesh;
}

std::shared_ptr<Material>
    ResourceManager::CreateMaterial(const std::string& _name, const std::string& _pathVertex, const std::string& _pathFrag, size_t _uniformBufferSize, std::vector<MirageRHI::UBOBlockInfo> _shader_blocks)
{
    ResourceManager& rm = ResourceManager::Instance();

    if (!rm.m_Rhi)
    {
        LOG(logError, LogResourceManager, "Resource Manager RHI isn't init");
        return nullptr;
    }
    std::string newAssetName = _name;
    auto resourceIt = rm.m_Resources.find(_name);
    if (resourceIt != rm.m_Resources.end())
    {
        newAssetName = rm.GenerateUniqueAssetName(_name);
    }

    LOG(logTemp, LogResourceManager, "Creating Material : ", newAssetName);

    std::shared_ptr<Shader> frag
        = rm.m_ShaderCache.contains(_pathFrag) ? rm.m_ShaderCache[_pathFrag] : ResourceManager::CreateShader("ShaderFragment", _pathFrag, MirageRHI::IShader::ShaderType::Fragment);

    std::shared_ptr<Shader> vert
        = rm.m_ShaderCache.contains(_pathVertex) ? rm.m_ShaderCache[_pathVertex] : ResourceManager::CreateShader("ShaderVertex", _pathVertex, MirageRHI::IShader::ShaderType::Vertex);

    std::shared_ptr<MirageRHI::IPipelineParameters> pipelineparameter = rm.m_Rhi->InstantiatePipelineParameters();
    pipelineparameter->Create(MirageRHI::CullingType::None, true); // Default setting

    std::shared_ptr<MirageRHI::IPipeline> pipeline = rm.m_Rhi->InstantiatePipeline();
    pipeline->Create(vert->ShaderBind, frag->ShaderBind, pipelineparameter, _shader_blocks);

    std::shared_ptr<Material> material = std::make_shared<Material>();
    material->p_Name = newAssetName;
    material->p_Path = ""; // No path for material as it's created empty
    material->fragmentShader = frag->ShaderBind;
    material->vertexShader = vert->ShaderBind;
    material->uniformBufferSize = _uniformBufferSize;
    material->pipeline = pipeline;
    rm.m_Resources.emplace(newAssetName, material);
    return material;
}

std::shared_ptr<MaterialInstance> ResourceManager::CreateMaterialInstance(const std::string& _name, std::shared_ptr<Material> mat)
{
    ResourceManager& rm = ResourceManager::Instance();

    if (!rm.m_Rhi)
    {
        LOG(logError, LogResourceManager, "Resource Manager RHI isn't init");
        return nullptr;
    }
    LOG(logTemp, LogResourceManager, "Creating Material Instance from : ", mat->GetName());
    std::string newAssetName = _name;
    auto resourceIt = rm.m_Resources.find(_name);
    if (resourceIt != rm.m_Resources.end())
    {
        newAssetName = rm.GenerateUniqueAssetName(_name);
    }

    std::shared_ptr<MaterialInstance> matInstance = std::make_shared<MaterialInstance>(mat);

    matInstance->p_Name = newAssetName;
    matInstance->p_Path = ""; // No path for material instance as it's created from material
    rm.m_Resources.emplace(newAssetName, matInstance);
    return matInstance;
}

std::shared_ptr<Pipeline> ResourceManager::CreatePipeline(
    const std::string& _name, const std::string& _pathFrag, const std::string& _pathVertex, std::shared_ptr<MirageRHI::IPipelineParameters> _pipelineParamter,
    std::vector<MirageRHI::UBOBlockInfo> _uboBlockInfo)
{
    ResourceManager& rm = ResourceManager::Instance();

    if (!rm.m_Rhi)
    {
        LOG(logError, LogResourceManager, "Resource Manager RHI isn't init");
        return nullptr;
    }
    std::string newAssetName = _name;
    auto resourceIt = rm.m_Resources.find(_name);
    if (resourceIt != rm.m_Resources.end())
    {
        newAssetName = rm.GenerateUniqueAssetName(_name);
    }

    std::shared_ptr<Shader> frag
        = rm.m_ShaderCache.contains(_pathFrag) ? rm.m_ShaderCache[_pathFrag] : ResourceManager::CreateShader("ShaderFragment", _pathFrag, MirageRHI::IShader::ShaderType::Fragment);

    std::shared_ptr<Shader> vert
        = rm.m_ShaderCache.contains(_pathVertex) ? rm.m_ShaderCache[_pathVertex] : ResourceManager::CreateShader("ShaderVertex", _pathVertex, MirageRHI::IShader::ShaderType::Vertex);

    std::shared_ptr<Pipeline> pipeline = std::make_shared<Pipeline>(rm.m_Rhi->InstantiatePipeline(), _pipelineParamter, vert, frag, _uboBlockInfo);
    pipeline->p_Name = newAssetName;
    pipeline->p_Path = ""; // No path for pipeline as it's created from shaders
    rm.m_Resources.emplace(newAssetName, pipeline);
    return pipeline;
}

std::string ResourceManager::GenerateUniqueAssetName(const std::string& _baseName)
{
    std::string uniqueName = _baseName;
    int counter = 1;
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

void ResourceManager::LoadMeshUtility(std::string _path, std::vector<LitVertex>& _vertices, std::vector<uint32_t>& _indices)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, _path.c_str());

    if (!err.empty())
    {
        LOG(logWarning, LogResourceManager, err);
    }
    if (!ret)
    {
        LOG(logWarning, LogResourceManager, "Failed to load OBJ : ", _path);
        return;
    }

    size_t totalIndices = 0;
    for (const auto& shape : shapes)
    {
        totalIndices += shape.mesh.indices.size();
    }

    _vertices.reserve(totalIndices);
    _indices.reserve(totalIndices);
    uint32_t vertexIndex = static_cast<uint32_t>(_vertices.size());

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

                LitVertex vertex;

                // Position
                vertex.Position = MirageEngine::Core::Math::Vector3(attrib.vertices[3 * idx.vertex_index + 0], attrib.vertices[3 * idx.vertex_index + 1], attrib.vertices[3 * idx.vertex_index + 2]);

                // Normal
                if (idx.normal_index >= 0)
                {
                    vertex.Normal = MirageEngine::Core::Math::Vector3(attrib.normals[3 * idx.normal_index + 0], attrib.normals[3 * idx.normal_index + 1], attrib.normals[3 * idx.normal_index + 2]);
                }
                else
                {
                    vertex.Normal = MirageEngine::Core::Math::Vector3(0, 0, 0);
                }

                // UV
                if (idx.texcoord_index >= 0)
                {
                    vertex.UV = MirageEngine::Core::Math::Vector2(attrib.texcoords[2 * idx.texcoord_index + 0], attrib.texcoords[2 * idx.texcoord_index + 1]);
                }
                else
                {
                    vertex.UV = MirageEngine::Core::Math::Vector2(0, 0);
                }

                _vertices.push_back(vertex);

                faceIndices.push_back(vertexIndex++);
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

void ResourceManager::LoadTextureUtility(const std::string& _path, int& _width, int& _height, int& _channel, int& _format, unsigned char*& _data)
{
    stbi_set_flip_vertically_on_load(true);

    _data = stbi_load(_path.c_str(), &_width, &_height, &_channel, 0);
    if (!_data)
    {
        LOG(logError, LogTexture, "Failed to load texture from path: ", _path);
        return;
    }

    _format = (_channel == 4) ? OPENGL_RGBA : OPENGL_RGB;
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& _name)
{
    ResourceManager& resourceManager = ResourceManager::Instance();
    auto it = resourceManager.m_Resources.find(_name);
    if (it != resourceManager.m_Resources.end())
    {
        if (std::shared_ptr<Texture> resource = std::dynamic_pointer_cast<Texture>(it->second))
            return resource;

        LOG(logWarning, LogResourceManager, "Resource type mismatch for name: ", _name);
    }
    else
    {
        LOG(logWarning, LogResourceManager, "Resource not found: ", _name);
        return resourceManager.PrivateGet<Texture>("DefaultTexture"); // Return default texture if not found
    }
    return nullptr;                                                   // Should never reach here, but added to avoid compiler warning
}

std::shared_ptr<Shader> ResourceManager::GetVertexShader(const std::string& _name)
{
    ResourceManager& resourceManager = ResourceManager::Instance();
    auto it = resourceManager.m_Resources.find(_name);
    if (it != resourceManager.m_Resources.end())
    {
        if (std::shared_ptr<Shader> resource = std::dynamic_pointer_cast<Shader>(it->second))
            return resource;
        LOG(logWarning, LogResourceManager, "Resource type mismatch for name: ", _name);
    }
    else
    {
        LOG(logWarning, LogResourceManager, "Resource not found: ", _name);
        return resourceManager.PrivateGet<Shader>("DefaultVertex"); // Return default vertex shader if not found
    }
    return nullptr;                                                 // Should never reach here, but added to avoid compiler warning
}

std::shared_ptr<Shader> ResourceManager::GetFragmentShader(const std::string& _name)
{
    ResourceManager& resourceManager = ResourceManager::Instance();
    auto it = resourceManager.m_Resources.find(_name);
    if (it != resourceManager.m_Resources.end())
    {
        if (std::shared_ptr<Shader> resource = std::dynamic_pointer_cast<Shader>(it->second))
            return resource;
        LOG(logWarning, LogResourceManager, "Resource type mismatch for name: ", _name);
    }
    else
    {
        LOG(logWarning, LogResourceManager, "Resource not found: ", _name, ", Using default Shader");
        return resourceManager.PrivateGet<Shader>("DefaultFragment"); // Return default fragment shader if not found
    }
    return nullptr;                                                   // Should never reach here, but added to avoid compiler warning
}

std::shared_ptr<Mesh> ResourceManager::GetMesh(const std::string& _name)
{
    ResourceManager& resourceManager = ResourceManager::Instance();
    auto it = resourceManager.m_Resources.find(_name);
    if (it != resourceManager.m_Resources.end())
    {
        if (std::shared_ptr<Mesh> resource = std::dynamic_pointer_cast<Mesh>(it->second))
            return resource;
        LOG(logWarning, LogResourceManager, "Resource type mismatch for name: ", _name);
    }
    // else
    //{
    //     LOG(logWarning, LogResourceManager, "Resource not found: ", _name);
    //     return resourceManager.PrivateGet<Mesh>("DefaultMesh"); // Return default mesh if not found
    // }
    LOG(logFatal, LogResourceManager, "Resource not found: ", _name);
    return nullptr;
}

std::shared_ptr<Material> ResourceManager::GetMaterial(const std::string& _name)
{
    ResourceManager& resourceManager = ResourceManager::Instance();
    auto it = resourceManager.m_Resources.find(_name);
    if (it != resourceManager.m_Resources.end())
    {
        if (std::shared_ptr<Material> resource = std::dynamic_pointer_cast<Material>(it->second))
            return resource;
        LOG(logWarning, LogResourceManager, "Resource type mismatch for name: ", _name);
    }
    LOG(logFatal, LogResourceManager, "Resource not found: ", _name);
    return nullptr;
}

std::shared_ptr<MaterialInstance> ResourceManager::GetMaterialInstance(const std::string& _name)
{
    ResourceManager& resourceManager = ResourceManager::Instance();
    auto it = resourceManager.m_Resources.find(_name);
    if (it != resourceManager.m_Resources.end())
    {
        if (std::shared_ptr<MaterialInstance> resource = std::dynamic_pointer_cast<MaterialInstance>(it->second))
            return resource;
        LOG(logWarning, LogResourceManager, "Resource type mismatch for name: ", _name);
    }
    LOG(logFatal, LogResourceManager, "Resource not found: ", _name);
    return nullptr;
}
