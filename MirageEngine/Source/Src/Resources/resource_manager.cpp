#include <Resources/resource_manager.h>
#include <RHI/OpenGL/openGL_buffer.h>
#include <RHI/OpenGL/openGL_pipeline.h>
#include <RHI/OpenGL/openGL_pipeline_parameters.h>
#include <RHI/OpenGL/openGL_shader.h>
#include <RHI/i_texture.h>
#include <RHI/i_render_interface.h>
#include <TinyObjLoader/tiny_obj_loader.h>

#define TINYOBJLOADER_IMPLEMENTATION

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

MirageRHI::ITexture* ResourceManager::CreateTexture(const std::string& , const std::string& )
{
	ResourceManager* rm = ResourceManager::Instance();
	if (!rm)
	{
		//LOG(logError, LogResourceManager, "Resource Manager isn't instanciate or doesn't exist in this context");
		return nullptr;
	}
	return nullptr;
}

MirageRHI::IShader* ResourceManager::CreateShader(const std::string& _name, const std::string& _path, const MirageRHI::IShader::ShaderType _type)
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
	std::string    newAssetName = _name;
	auto it = rm->m_resources.find(_name);
	if (it != rm->m_resources.end())
	{
		newAssetName = rm->GenerateUniqueAssetName(_name);
	}
	MirageRHI::IShader* shader = rm->m_rhi->InstantiateShader();
	shader->m_name = newAssetName;
	shader->Create(_path.c_str(), _type);
	rm->m_resources.emplace(newAssetName, shader); // global resource
	rm->m_shaderCache.emplace(_path, shader); // local cache
	return shader;
}

MirageRHI::IPipeline* ResourceManager::CreatePipeline(const std::string& _name, const char* _pathFrag, const char* _pathVertex, MirageRHI::IPipelineParameters* _pipelineParamter)
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
	std::string    newAssetName = _name;
	auto resourceIt = rm->m_resources.find(_name);
	if (resourceIt != rm->m_resources.end())
	{
		newAssetName = rm->GenerateUniqueAssetName(_name);
	}
	
	// pre define ptr
	MirageRHI::IShader* frag;
	MirageRHI::IShader* vert;

	// search in cache
	auto shaderFragIt = rm->m_shaderCache.find(_pathFrag);
	auto shaderVertIt = rm->m_shaderCache.find(_pathFrag);

	// If find get, else, load
	if (shaderFragIt != rm->m_shaderCache.end())
		frag = shaderFragIt->second.get();
	else
		frag = ResourceManager::CreateShader("ShaderFragment", _pathFrag, MirageRHI::IShader::ShaderType::Fragment);
	if (shaderVertIt != rm->m_shaderCache.end())
		vert = shaderVertIt->second.get();
	else
		vert = ResourceManager::CreateShader("ShaderVertex", _pathVertex, MirageRHI::IShader::ShaderType::Vertex);


	MirageRHI::IPipeline* pipeline = rm->m_rhi->InstantiatePipeline();

	pipeline->Create(frag,vert,_pipelineParamter);
	rm->m_resources.emplace(newAssetName, pipeline);
	return pipeline;
}

IMesh* ResourceManager::CreateMesh(const std::string& , const std::string& )
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

	//MirageRHI::IBuffer* m_BufferVertices = rm->m_rhi->InstantiateBuffer();
	//MirageRHI::IBuffer* m_BufferIndices = rm->m_rhi->InstantiateBuffer();
	return nullptr;
}

std::string ResourceManager::GenerateUniqueAssetName(const std::string& _baseName)
{
	std::string uniqueName = _baseName;
	int         counter = 1;
	while (m_resources.find(uniqueName) != m_resources.end())
	{
		std::stringstream ss;
		ss << _baseName << "_" << counter;
		uniqueName = ss.str();
		++counter;
	}
	//LOG(logWarning, LogResourceManager, _baseName, " already taken, replaced by ", uniqueName);

	return uniqueName;

}

IMesh::IMesh(MirageRHI::IBuffer* _vert, MirageRHI::IBuffer* _frag)
	: m_vertexBuffer(_vert), m_fragmentBuffer(_frag)
{
}

void ResourceManager::LoadMeshUtility(std::string)
{
	//tinyobj::attrib_t attrib;
	//std::vector<tinyobj::shape_t> shapes;
	//std::vector<tinyobj::material_t> materials;

	//std::string err;

	//bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, _path.c_str());

	//if (!err.empty()) {
	//	std::cerr << err << std::endl;
	//}

	//if (!ret) {
	//	exit(1);
	//}

	//// Loop over shapes
	//for (size_t s = 0; s < shapes.size(); s++) {
	//	// Loop over faces(polygon)
	//	size_t index_offset = 0;
	//	for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
	//		size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

	//		// Loop over vertices in the face.
	//		for (size_t v = 0; v < fv; v++) {
	//			// access to vertex
	//			tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

	//			tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
	//			tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
	//			tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

	//			// Check if `normal_index` is zero or positive. negative = no normal data
	//			if (idx.normal_index >= 0) {
	//				tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
	//				tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
	//				tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
	//			}

	//			// Check if `texcoord_index` is zero or positive. negative = no texcoord data
	//			if (idx.texcoord_index >= 0) {
	//				tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
	//				tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
	//			}
	//			// Optional: vertex colors
	//			// tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
	//			// tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
	//			// tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
	//		}
	//		index_offset += fv;

	//		// per-face material
	//		shapes[s].mesh.material_ids[f];
	//	}
	//}
}