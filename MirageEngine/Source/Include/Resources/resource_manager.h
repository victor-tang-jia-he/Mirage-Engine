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

				std::unordered_map<std::string, std::unique_ptr<IResource>> m_resources;

				std::unordered_map<std::string, std::shared_ptr<MirageRHI::IShader>> m_shaderCache;

				static ResourceManager* m_instance;

				MirageRHI::IRenderInterface* m_rhi = nullptr;

				void LoadMeshUtility(std::string _path, std::vector<MirageRHI::LitVertex>& _vertices, std::vector<uint32_t>& _indices);
			public:
				static ResourceManager* Instance();

				static void Clear();

				void Init(MirageRHI::IRenderInterface* _rhi);

				ResourceManager() = default;
				~ResourceManager();
				// -------------------------------------------------------------------------------- //    
				// Basic Constructor 
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Create a Texture
				/// </summary>
				/// <param name="name">Name given</param>
				/// <param name="path">File Path</param>
				/// <returns>Texture pointer</returns>
				static Texture* CreateTexture(const std::string& _name, const std::string& _path);

				static MirageRHI::IShader* CreateShader(const std::string& _name, const std::string& _path, const MirageRHI::IShader::ShaderType _type);

				/// <summary>
				/// Create a Shader
				/// </summary>
				/// <param name="name">Name given</param>
				/// <param name="pathFrag">Fragment file path</param>
				/// <param name="pathVertex">Vertex file path</param>
				/// <returns>Shader pointer</returns>
				static MirageRHI::IPipeline* CreatePipeline(const std::string& _name, const char* _pathFrag, const char* _pathVertex, MirageRHI::IPipelineParameters* _pipelineParamter);

				/// <summary>
				/// Create a Mesh
				/// </summary>
				/// <param name="name">Name given</param>
				/// <param name="path">File path</param>
				/// <returns>Mesh pointer</returns>
				static IMesh* CreateMesh(const std::string& _name, const std::string& _path);

				/// <summary>
				/// Get a resource from the the list by the name.
				/// </summary>
				/// <param name="name"> : Name of the resource in string.</param>
				/// <returns> Return an reference of the instance if it exist , otherwise it return nullptr </returns>
				template <typename T>
				static T* Get(const std::string& _name)
				{
					if (!std::is_base_of_v<MirageEngine::Core::Resources::IResource, T>) return nullptr;// T must inherit from IResource
					ResourceManager* resourceManager = ResourceManager::Instance();

					auto it = resourceManager->m_resources.find(_name);
					if (it != resourceManager->m_resources.end())
					{
						T* resource = dynamic_cast<T*>(it->second.get());
						return resource;
					}
					//LOG(logWarning, LogResourceManager,"Resource Manager", "Resource with name '%s' not found.", _name.c_str());
					return nullptr;
				}

				/// <summary>
				/// Delete an resources if it exist by the name
				/// </summary>
				/// <param name="name"> : Name of the resource in string.</param>
				/// <returns> Return true if find and deleted, return false if resources doesn't exist. </returns>
				inline bool Delete(const std::string& _name)
				{
					return m_resources.erase(_name) > 0;
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
			};
		}
	}
}