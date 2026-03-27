#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <Resources/resource_interface.h>
#include <RHI/Include/i_shader.h>
#include <memory>
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
}

namespace MirageEngine
{
	namespace Core
	{
		namespace Resources
		{
			class IMesh
			{
				MirageRHI::IBuffer* m_vertexBuffer;
				MirageRHI::IBuffer* m_fragmentBuffer;

			public:
				IMesh(MirageRHI::IBuffer* vert, MirageRHI::IBuffer* frag);
			};

			class ResourceManager
			{
				friend class IResource;

				std::unordered_map<std::string, std::unique_ptr<IResource>> m_resources;
				
				std::unordered_map<std::string, std::shared_ptr<MirageRHI::IShader>> m_shaderCache;

				static ResourceManager* m_instance;

				MirageRHI::IRenderInterface* m_rhi = nullptr;
			public:
				static ResourceManager* Instance();

				static void Clear();

				void Init(MirageRHI::IRenderInterface* _rhi);

				ResourceManager() = default;
				~ResourceManager() = default;
				// -------------------------------------------------------------------------------- //    
				// Basic Constructor 
				// -------------------------------------------------------------------------------- //

				/// <summary>
				/// Create a Texture
				/// </summary>
				/// <param name="name">Name given</param>
				/// <param name="path">File Path</param>
				/// <returns>Texture pointer</returns>
				static MirageRHI::ITexture* CreateTexture(const std::string& name, const std::string& path);

				static MirageRHI::IShader* CreateShader(const std::string& name, const std::string& path, const MirageRHI::IShader::ShaderType type);

				/// <summary>
				/// Create a Shader
				/// </summary>
				/// <param name="name">Name given</param>
				/// <param name="pathFrag">Fragment file path</param>
				/// <param name="pathVertex">Vertex file path</param>
				/// <returns>Shader pointer</returns>
				static MirageRHI::IPipeline* CreatePipeline(const std::string& name,  const char* pathFrag,  const char* pathVertex, MirageRHI::IPipelineParameters* pipelineParamter);

				/// <summary>
				/// Create a Mesh
				/// </summary>
				/// <param name="name">Name given</param>
				/// <param name="path">File path</param>
				/// <returns>Mesh pointer</returns>
				static IMesh* CreateMesh(const std::string& name, const std::string& path);

				/// <summary>
				/// Get a resource from the the list by the name.
				/// </summary>
				/// <param name="name"> : Name of the resource in string.</param>
				/// <returns> Return an reference of the instance if it exist , otherwise it return nullptr </returns>
				template <typename T>
				static T* Get(const std::string& name)
				{
					if (!std::is_base_of_v<IResource, T>) return nullptr;// T must inherit from IResource
					ResourceManager* resourceManager = ResourceManager::Instance();

					auto it = resourceManager->m_resources.find(name);
					if (it != resourceManager->m_resources.end())
					{
						T* resource = static_cast<T*>(it->second);
						return resource;
					}
					return nullptr;
				}

				/// <summary>
				/// Delete an resources if it exist by the name
				/// </summary>
				/// <param name="name"> : Name of the resource in string.</param>
				/// <returns> Return true if find and deleted, return false if resources doesn't exist. </returns>
				inline bool Delete(const std::string& name)
				{
					return m_resources.erase(name) > 0;
				}

			private:
				/// <summary>
				/// Generate a Unique name
				/// </summary>
				/// <param name="baseName">Original name</param>
				/// <returns>New name exemple : (enter)resourceName -> (OutPut) resourceName_1 </returns>
				std::string GenerateUniqueAssetName(const std::string& baseName);

				// Source - https://stackoverflow.com/a
				// Posted by Max, modified by community. See post 'Timeline' for change history
				// Retrieved 2026-01-28, License - CC BY-SA 4.0

				/// <summary>
				/// Remove an element from a vector list
				/// </summary>
				/// <param name="vec">the vector </param>
				/// <param name="pos">the index of the element need to be removed</param>
				template <typename T>
				static void remove(std::vector<T>& vec, std::size_t pos)
				{
					if (pos < vec.size()) {
						typename std::vector<T>::iterator it = vec.begin();
						std::advance(it, pos);
						vec.erase(it);
					}
				}
			};
		}
	}
}