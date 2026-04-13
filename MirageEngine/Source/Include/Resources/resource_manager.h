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

namespace MirageEngine::Core::Resources
{
	class Texture;
	class Mesh;
	class Material;
	struct Shader;
	struct Pipeline;

	class ResourceManager
	{
		friend class IResource;

	private:

		std::unordered_map<std::string, std::shared_ptr<IResource>>          m_Resources;
		std::unordered_map<std::string, std::shared_ptr<MirageRHI::IShader>> m_ShaderCache;

		static ResourceManager* m_Instance;
		MirageRHI::IRenderInterface* m_Rhi = nullptr;

	public:

		ResourceManager() = default;
		~ResourceManager();

		// -------------------------------------------------------------------------
		// Lifecycle
		// -------------------------------------------------------------------------

		static ResourceManager* Instance();

		/// <summary>
		/// Initializes the ResourceManager with the given render interface.
		/// Must be called once before any resource creation.
		/// </summary>
		/// <param name="_rhi">Pointer to the active render interface.</param>
		void Init(MirageRHI::IRenderInterface* _rhi);

		/// <summary>
		/// Pre-loads a set of essential resources at startup and all resource from Assets folder.
		/// </summary>
		void PreLoadResources();

		// -------------------------------------------------------------------------
		// Resource creation
		// -------------------------------------------------------------------------

		/// <summary>
		/// Creates a texture resource and registers it in the ResourceManager.
		/// </summary>
		/// <param name="_name">Unique identifier for the texture.</param>
		/// <param name="_path">File path to the texture on disk.</param>
		/// <returns>Pointer to the created Texture, or nullptr on failure.</returns>
		static Texture* CreateTexture(const std::string& _name, const std::string& _path);

		/// <summary>
		/// Creates a shader resource from a file and registers it.
		/// </summary>
		/// <param name="_name">Unique identifier for the shader.</param>
		/// <param name="_path">File path to the shader source.</param>
		/// <param name="_type">Type of shader (e.g., vertex, fragment).</param>
		/// <returns>Pointer to the created Shader, or nullptr on failure.</returns>
		static Shader* CreateShader(const std::string& _name, const std::string& _path, MirageRHI::IShader::ShaderType _type);

		/// <summary>
		/// Loads a mesh from file, creates the GPU buffers, and registers it.
		/// </summary>
		/// <param name="_name">Unique identifier for the mesh.</param>
		/// <param name="_path">File path to the mesh asset (e.g., .obj).</param>
		/// <returns>Pointer to the created Mesh, or nullptr on failure.</returns>
		static Mesh* CreateMesh(const std::string& _name, const std::string& _path);

		/// <summary>
		/// Creates an empty material and registers it.
		/// </summary>
		/// <param name="_name">Unique identifier for the material.</param>
		/// <returns>Pointer to the created Material, or nullptr on failure.</returns>
		static Material* CreateMaterial(const std::string& _name);

		/// <summary>
		/// Creates an empty material and registers it.
		/// </summary>
		/// <param name="_name">Unique identifier for the material.</param>
		/// <returns>Pointer to the created Material, or nullptr on failure.</returns>
		static Pipeline* CreatePipeline(const std::string& _name, const char* _pathFrag, const char* _pathVertex, MirageRHI::IPipelineParameters* _pipelineParamter);

		// -------------------------------------------------------------------------
		// Resource access
		// -------------------------------------------------------------------------

		Texture* GetTexture(const std::string& _name);
		Shader* GetVertexShader(const std::string& _name);
		Shader* GetFragmentShader(const std::string& _name);
		Mesh* GetMesh(const std::string& _name);
		Material* GetMaterial(const std::string& _name);

		/// <summary>
		/// Retrieves a resource by name and casts it to the requested type.
		/// </summary>
		/// <typeparam name="T">Expected resource type (must inherit from IResource).</typeparam>
		/// <param name="_name">Unique identifier of the resource.</param>
		/// <param name="out">Output pointer set to the retrieved resource, or nullptr on failure.</param>
		/// <returns>True if the resource was found and the cast succeeded, false otherwise.</returns>
		template <typename T>
		static bool TryGet(const std::string& _name, T*& out)
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

		// -------------------------------------------------------------------------
		// Resource removal
		// -------------------------------------------------------------------------

		/// <summary>
		/// Removes a resource from the manager by name.
		/// </summary>
		/// <param name="_name">Unique identifier of the resource to remove.</param>
		/// <returns>True if the resource was found and removed, false otherwise.</returns>
		inline bool Delete(const std::string& _name) { return m_Resources.erase(_name) > 0; }

	private:

		// -------------------------------------------------------------------------
		// Internal utilities
		// -------------------------------------------------------------------------

		/// <summary>
		/// Generates a unique name by appending an incremented suffix if the base name is already taken.
		/// </summary>
		/// <param name="_baseName">Desired resource name.</param>
		/// <returns>A unique name (e.g., "resourceName" becomes "resourceName_1").</returns>
		std::string GenerateUniqueAssetName(const std::string& _baseName);

		void LoadMeshUtility(std::string _path, std::vector<MirageRHI::LitVertex>& _vertices, std::vector<uint32_t>& _indices);

		void LoadTextureUtility(const std::string& _path, int& m_Width, int& m_Height, int& m_Channel, int& m_Format, unsigned char*& m_Data);

		/// <summary>
		/// Retrieves a resource by name and casts it to the requested type.
		/// For internal use only ¡ª prefer TryGet or the type-specific getters for external access.
		/// </summary>
		/// <typeparam name="T">Expected resource type (must inherit from IResource).</typeparam>
		/// <param name="_name">Unique identifier of the resource.</param>
		/// <returns>Pointer to the resource, or nullptr if not found or the cast failed.</returns>
		template <typename T>
		T* PrivateGet(const std::string& _name)
		{
			static_assert(std::is_base_of_v<IResource, T>, "T must derive from IResource");

			auto it = m_Resources.find(_name);
			if (it != m_Resources.end())
			{
				if (T* resource = dynamic_cast<T*>(it->second.get())) return resource;

				LOG(logWarning, LogResourceManager, "Resource type mismatch for name: ", _name);
			}

			return nullptr;
		}
	};

} // namespace MirageEngine::Core::Resources