#include <Resources/resource_manager.h>
#include <RHI/Include/OpenGL/openGL_buffer.h>
#include <RHI/Include/OpenGL/openGL_pipeline.h>
#include <RHI/Include/OpenGL/openGL_pipeline_parameters.h>
#include <RHI/Include/OpenGL/openGL_shader.h>
#include <RHI/Include/i_texture.h>
#include <RHI/Include/i_render_interface.h>

//DEFINE_LOG_CATEGORY(LogResourceManager)

using namespace MirageEngine::Core::Resources;

ResourceManager* ResourceManager::m_instance = nullptr;

ResourceManager* ResourceManager::Instance()
{
	if (m_instance == nullptr)
		m_instance = new ResourceManager();
	return m_instance;
}


void ResourceManager::Init(MirageRHI::IRenderInterface* _rhi)
{
	m_rhi = _rhi; // set for later creating resource
}

MirageRHI::ITexture* ResourceManager::CreateTexture(const std::string& name, const std::string& path)
{
	ResourceManager* rm = ResourceManager::Instance();
	if (!rm)
	{
		//LOG(logError, LogResourceManager, "Resource Manager isn't instanciate or doesn't exist in this context");
		return nullptr;
	}
	return nullptr;
}

MirageRHI::IShader* ResourceManager::CreateShader(const std::string& name, const std::string& path, const MirageRHI::IShader::ShaderType type)
{
	ResourceManager* rm = ResourceManager::Instance();
	if (!rm)
	{
		//LOG(logWarning, LogResourceManager, "Resource Manager isn't instanciate or doesn't exist in this context");
		return nullptr;
	}
	if (!rm->m_rhi)
	{
		//LOG(logError, LogResourceManager, "Resource Manager RHI isn't init");
		return nullptr;
	}
	std::string    newAssetName = name;
	auto it = rm->m_resources.find(name);
	if (it != rm->m_resources.end())
	{
		newAssetName = rm->GenerateUniqueAssetName(name);
	}
	MirageRHI::IShader* shader = rm->m_rhi->InstantiateShader();
	shader->m_name = newAssetName;
	shader->Create(path.c_str(), type);
	rm->m_resources.emplace(newAssetName, shader); // global resource
	rm->m_shaderCache.emplace(path, shader); // local cache
	return shader;
}

MirageRHI::IPipeline* ResourceManager::CreatePipeline(const std::string& name, const char* pathFrag, const char* pathVertex, MirageRHI::IPipelineParameters* _pipelineParamter)
{
	ResourceManager* rm = ResourceManager::Instance();
	if (!rm)
	{
		//LOG(logWarning, LogResourceManager, "Resource Manager isn't instanciate or doesn't exist in this context");
		return nullptr;
	}
	if (!rm->m_rhi)
	{
		//LOG(logError, LogResourceManager, "Resource Manager RHI isn't init");
		return nullptr;
	}
	std::string    newAssetName = name;
	auto resourceIt = rm->m_resources.find(name);
	if (resourceIt != rm->m_resources.end())
	{
		newAssetName = rm->GenerateUniqueAssetName(name);
	}
	
	// pre define ptr
	MirageRHI::IShader* frag;
	MirageRHI::IShader* vert;

	// search in cache
	auto shaderFragIt = rm->m_shaderCache.find(pathFrag);
	auto shaderVertIt = rm->m_shaderCache.find(pathFrag);

	// If find get, else, load
	if (shaderFragIt != rm->m_shaderCache.end())
		frag = shaderFragIt->second.get();
	else
		frag = ResourceManager::CreateShader("ShaderFragment", pathFrag, MirageRHI::IShader::ShaderType::Fragment);
	if (shaderVertIt != rm->m_shaderCache.end())
		vert = shaderVertIt->second.get();
	else
		vert = ResourceManager::CreateShader("ShaderVertex", pathVertex, MirageRHI::IShader::ShaderType::Vertex);


	MirageRHI::IPipeline* pipeline = rm->m_rhi->InstantiatePipeline();

	pipeline->Create(frag,vert,_pipelineParamter);
	rm->m_resources.emplace(newAssetName, pipeline);
	return pipeline;
}

IMesh* ResourceManager::CreateMesh(const std::string& name, const std::string& path)
{
	ResourceManager* rm = ResourceManager::Instance();
	if (!rm)
	{
		//LOG(logWarning, LogResourceManager, "Resource Manager isn't instanciate or doesn't exist in this context");
		return nullptr;
	}
	if (!rm->m_rhi)
	{
		//LOG(logError, LogResourceManager, "Resource Manager RHI isn't init");
		return nullptr;
	}

	MirageRHI::IBuffer* m_BufferVertices = rm->m_rhi->InstantiateBuffer();
	MirageRHI::IBuffer* m_BufferIndices = rm->m_rhi->InstantiateBuffer();
	return nullptr;
}

std::string ResourceManager::GenerateUniqueAssetName(const std::string& baseName)
{
	std::string uniqueName = baseName;
	int         counter = 1;
	while (m_resources.find(uniqueName) != m_resources.end())
	{
		std::stringstream ss;
		ss << baseName << "_" << counter;
		uniqueName = ss.str();
		++counter;
	}
	//LOG(logWarning, LogResourceManager, baseName, " already taken, replaced by ", uniqueName);

	return uniqueName;

}

IMesh::IMesh(MirageRHI::IBuffer* vert, MirageRHI::IBuffer* frag)
	: m_vertexBuffer(vert), m_fragmentBuffer(frag)
{
}
