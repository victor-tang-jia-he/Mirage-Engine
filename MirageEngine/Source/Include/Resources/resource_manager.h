#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <Resources/resource_interface.h>
#include <RHI/i_shader.h>
#include <memory>
#include <vector3.h>
#include <filesystem>

#include <Logs/log_macro.h>

DECLARE_LOG_CATEGORY_EXTERN(LogResourceManager)

namespace MirageRHI
{
    class IShader;
    class ITexture;
    class IBuffer;
    class IRenderInterface;
    struct IPipelineParameters;
    struct LitVertex;
} // namespace MirageRHI

namespace MirageEngine
{
    namespace Core
    {
        namespace Resources
        {
            class Texture;
            class Mesh;
            class Material;
            struct Shader;

            class ResourceManager
            {
                    friend class IResource;

                private:

                    std::unordered_map<std::string, std::shared_ptr<IResource>> m_Resources;

                    std::unordered_map<std::string, std::shared_ptr<MirageRHI::IShader>> m_ShaderCache;

                    static ResourceManager* m_Instance;

                    MirageRHI::IRenderInterface* m_Rhi = nullptr;

                public:

                    static ResourceManager* Instance();

                    void Init(MirageRHI::IRenderInterface* _rhi);

                    void PreLoadResources();

                    ResourceManager() = default;
                    ~ResourceManager();

                    /// <summary>
                    /// Creates a texture resource and registers it in the ResourceManager.
                    /// </summary>
                    /// <param name="_name">Unique identifier of the texture.</param>
                    /// <param name="_path">File path to the texture on disk.</param>
                    /// <returns>Pointer to the created Texture, or nullptr on failure.</returns>
                    static Texture* CreateTexture(const std::string& _name, const std::string& _path);

                    /// <summary>
                    /// Creates a shader resource from a file and registers it.
                    /// </summary>
                    /// <param name="_name">Unique identifier of the shader.</param>
                    /// <param name="_path">File path to the shader source.</param>
                    /// <param name="_type">Type of shader (e.g., vertex, fragment).</param>
                    /// <returns>Pointer to the created shader, or nullptr on failure.</returns>
                    static Shader* CreateShader(const std::string& _name, const std::string& _path, const MirageRHI::IShader::ShaderType _type);

                    /// <summary>
                    /// Loads a mesh from file, creates GPU buffers, and registers it.
                    /// </summary>
                    /// <param name="_name">Unique identifier of the mesh.</param>
                    /// <param name="_path">File path to the mesh (e.g., OBJ).</param>
                    /// <returns>Pointer to the created mesh, or nullptr on failure.</returns>
                    static Mesh* CreateMesh(const std::string& _name, const std::string& _path);

                    static Material* CreateMaterial(const std::string& _name);

                    Texture* GetTexture(const std::string& _name);

                    Shader* GetVertexShader(const std::string& _name);

                    Shader* GetFragmentShader(const std::string& _name);

                    Mesh* GetMesh(const std::string& _name);

                    Material* GetMaterial(const std::string& _name);

                    /// <summary>
                    /// Retrieves a resource by name and casts it to the requested type.
                    /// </summary>
                    /// <typeparam name="T">Expected resource type (must inherit from IResource).</typeparam>
                    /// <typeparam name="T">Output parameter to store the retrieved resource.</typeparam>
                    /// <param name="_name">Name of the resource.</param>
                    /// <returns>
                    /// True if found and valid,
                    /// otherwise False.
                    /// </returns>
                    template <typename T> static bool TryGet(const std::string& _name, T*& out)
                    {
                        if (!std::is_base_of_v<MirageEngine::Core::Resources::IResource, T>) return false;
                        ResourceManager* resourceManager = ResourceManager::Instance();

                        auto it = resourceManager->m_Resources.find(_name);
                        if (it != resourceManager->m_Resources.end())
                        {
                            out = dynamic_cast<T*>(it->second.get());
                            return out != nullptr;
                        }

                        out = nullptr;
                        return false;
                    }

                    /// <summary>
                    /// Removes a resource from the manager by name.
                    /// </summary>
                    /// <param name="_name">Name of the resource to delete.</param>
                    /// <returns>True if the resource was found and removed, false otherwise.</returns>
                    inline bool Delete(const std::string& _name) { return m_Resources.erase(_name) > 0; }

                private:

                    /// <summary>
                    /// Generate a Unique name
                    /// </summary>
                    /// <param name="baseName">Original name</param>
                    /// <returns>New name exemple : (enter)resourceName -> (OutPut) resourceName_1 </returns>
                    std::string GenerateUniqueAssetName(const std::string& _baseName);

                    // Source - https://stackoverflow.com/a
                    // Posted by Max, modified by community. See post 'Timeline' for change history
                    // Retrieved 2026-01-28, License - CC BY-SA 4.0

                    /// <summary>
                    /// Remove an element from a vector list
                    /// </summary>
                    /// <param name="_vec">the vector </param>
                    /// <param name="_pos">the index of the element need to be removed</param>
                    template <typename T> static void Remove(std::vector<T>& _vec, std::size_t _pos)
                    {
                        if (_pos < _vec.size())
                        {
                            typename std::vector<T>::iterator it = _vec.begin();
                            std::advance(it, _pos);
                            _vec.erase(it);
                        }
                    }

                    void LoadMeshUtility(std::string _path, std::vector<MirageRHI::LitVertex>& _vertices, std::vector<uint32_t>& _indices);

                    void LoadTextureUtility(const std::string& _path);


                    /// <summary>
                    /// Retrieves a resource by name and casts it to the requested type.
                    /// Only for internal use, prefer type-specific getters or TryGet for safer access.
                    template <typename T> T* PrivateGet(const std::string& _name)
                    {
                        static_assert(std::is_base_of_v<IResource, T>, "T must derive from IResource");
                        ResourceManager* resourceManager = ResourceManager::Instance();

                        auto it = resourceManager->m_Resources.find(_name);
                        if (it != resourceManager->m_Resources.end())
                        {
                            if (T* resource = dynamic_cast<T*>(it->second.get())) return resource;

                            LOG(logWarning, LogResourceManager, "Resource type mismatch for name: ", _name);
                        }

                        return nullptr;
                    }
            };
        } // namespace Resources
    } // namespace Core
} // namespace MirageEngine
