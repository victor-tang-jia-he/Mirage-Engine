#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <Resources/resource_interface.h>
#include <RHI/i_shader.h>
#include <memory>
#include <vector3.h>
//#include <Logs/log_macro.h>

//DECLARE_LOG_CATEGORY_EXTERN(LogResourceManager)

namespace MirageRHI
{
	class IPipeline;
	class IShader;
	class ITexture;
	class IBuffer;
	class IRenderInterface;
	struct IPipelineParameters;
	struct LitVertex;
}

namespace MirageEngine
{
	namespace Core
	{
		namespace Resources
		{	
			class Texture;
			class IMesh;

			class ResourceManager
			{
				friend class IResource;

			private:
				std::unordered_map<std::string, std::unique_ptr<IResource>> m_Resources;

				std::unordered_map<std::string, std::shared_ptr<MirageRHI::IShader>> m_ShaderCache;

				static ResourceManager* m_Instance;

				MirageRHI::IRenderInterface* m_Rhi = nullptr;

			public:
				static ResourceManager* Instance();

				void Init(MirageRHI::IRenderInterface* _rhi);

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
				static MirageRHI::IShader* CreateShader(const std::string& _name, const std::string& _path, const MirageRHI::IShader::ShaderType _type);


				/// <summary>
				/// Creates a rendering pipeline using vertex and fragment shaders.
				/// </summary>
				/// <param name="_name">Unique identifier of the pipeline.</param>
				/// <param name="_pathFrag">File path to the fragment shader.</param>
				/// <param name="_pathVertex">File path to the vertex shader.</param>
				/// <param name="_pipelineParamter">Pipeline configuration parameters.</param>
				/// <returns>Pointer to the created pipeline, or nullptr on failure.</returns>
				static MirageRHI::IPipeline* CreatePipeline(const std::string& _name, const char* _pathFrag, const char* _pathVertex, MirageRHI::IPipelineParameters* _pipelineParamter);


				/// <summary>
				/// Loads a mesh from file, creates GPU buffers, and registers it.
				/// </summary>
				/// <param name="_name">Unique identifier of the mesh.</param>
				/// <param name="_path">File path to the mesh (e.g., OBJ).</param>
				/// <returns>Pointer to the created mesh, or nullptr on failure.</returns>
				static IMesh* CreateMesh(const std::string& _name, const std::string& _path);


				/// <summary>
				/// Retrieves a resource by name and casts it to the requested type.
				/// </summary>
				/// <typeparam name="T">Expected resource type (must inherit from IResource).</typeparam>
				/// <param name="_name">Name of the resource.</param>
				/// <returns>
				/// Pointer to the resource casted to T if found and valid,
				/// otherwise nullptr.
				/// </returns>
				template <typename T>
				static T* Get(const std::string& _name)
				{
					if (!std::is_base_of_v<MirageEngine::Core::Resources::IResource, T>) return nullptr;

					ResourceManager* resourceManager = ResourceManager::Instance();

					auto it = resourceManager->m_Resources.find(_name);
					if (it != resourceManager->m_Resources.end())
					{
						T* resource = dynamic_cast<T*>(it->second.get());
						return resource;
					}

					return nullptr;
				}


				/// <summary>
				/// Removes a resource from the manager by name.
				/// </summary>
				/// <param name="_name">Name of the resource to delete.</param>
				/// <returns>True if the resource was found and removed, false otherwise.</returns>
				inline bool Delete(const std::string& _name)
				{
					return m_Resources.erase(_name) > 0;
				}

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
				template <typename T>
				static void remove(std::vector<T>& _vec, std::size_t _pos)
				{
					if (_pos < _vec.size())
					{
						typename std::vector<T>::iterator it = _vec.begin();
						std::advance(it, _pos);
						_vec.erase(it);
					}
				}

				void LoadMeshUtility(std::string _path, std::vector<MirageRHI::LitVertex>& _vertices, std::vector<uint32_t>& _indices);
			};
		}
	}
}