#include <Resources/resource_manager.h>
#include <RHI/OpenGL/openGL_buffer.h>
#include <RHI/OpenGL/openGL_pipeline.h>
#include <RHI/OpenGL/openGL_pipeline_parameters.h>
#include <RHI/OpenGL/openGL_shader.h>
#include <RHI/i_texture.h>
#include <RHI/i_render_interface.h>

#include <Resources/mesh.h>
#include <Resources/texture.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <TinyObjLoader/tiny_obj_loader.h>

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

Texture* ResourceManager::CreateTexture(const std::string& _name, const std::string& _path)
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
	MirageRHI::IBuffer* textureBuffer = rm->m_rhi->InstantiateBuffer();

	textureBuffer->Create(_path, MirageRHI::IBuffer::BufferType::Texture);

	Texture* texture = new Texture(textureBuffer);
	texture->m_name = newAssetName;
	rm->m_resources.emplace(newAssetName, texture); 
	return texture;
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
	auto shaderVertIt = rm->m_shaderCache.find(_pathVertex);

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

	pipeline->Create(frag, vert, _pipelineParamter);
	rm->m_resources.emplace(newAssetName, pipeline);
	return pipeline;
}

IMesh* ResourceManager::CreateMesh(const std::string& _name, const std::string& _path)
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

	MirageRHI::IBuffer* m_BufferVertices = rm->m_rhi->InstantiateBuffer();
	MirageRHI::IBuffer* m_BufferIndices = rm->m_rhi->InstantiateBuffer();

	std::vector<uint32_t> indices;
	std::vector<MirageRHI::LitVertex> vertices;

	rm->LoadMeshUtility(_path, vertices, indices);
	std::cout << "indices.size(): " << indices.size() << std::endl;
	std::cout << "expected bytes: " << indices.size() * sizeof(uint32_t) << std::endl;
	std::cout << "_size: " << indices.size() << std::endl;
	m_BufferVertices->Create(vertices.data(), vertices.size() * sizeof(MirageRHI::LitVertex), MirageRHI::IBuffer::BufferType::Vertices);
	m_BufferIndices->Create(indices.data(), indices.size() * sizeof(uint32_t), MirageRHI::IBuffer::BufferType::Indices);
	IMesh* mesh = new IMesh(m_BufferVertices, m_BufferIndices, indices.size());
	
	mesh->m_name = newAssetName;
	rm->m_resources.emplace(newAssetName, mesh);
	return mesh;
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

void ResourceManager::LoadMeshUtility(std::string _path,
	std::vector<MirageRHI::LitVertex>& _vertices,
	std::vector<uint32_t>& indices)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, _path.c_str());

	if (!err.empty()) {
		std::cerr << "ERROR: " << err << std::endl;
	}
	if (!ret) {
		std::cerr << "Failed to load OBJ: " << _path << std::endl;
		return;
	}

	size_t totalIndices = 0;
	for (const auto& shape : shapes) {
		totalIndices += shape.mesh.indices.size();
	}

	_vertices.reserve(totalIndices);
	indices.reserve(totalIndices);  

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
				vertex.Position = MirageEngine::Core::Math::Vector3(
					attrib.vertices[3 * idx.vertex_index + 0],
					attrib.vertices[3 * idx.vertex_index + 1],
					attrib.vertices[3 * idx.vertex_index + 2]
				);

				// Normal
				if (idx.normal_index >= 0) {
					vertex.Normal = MirageEngine::Core::Math::Vector3(
						attrib.normals[3 * idx.normal_index + 0],
						attrib.normals[3 * idx.normal_index + 1],
						attrib.normals[3 * idx.normal_index + 2]
					);
				}
				else {
					vertex.Normal = MirageEngine::Core::Math::Vector3(0, 0, 0);
				}

				// UV
				if (idx.texcoord_index >= 0) {
					vertex.UV = MirageEngine::Core::Math::Vector2(
						attrib.texcoords[2 * idx.texcoord_index + 0],
						attrib.texcoords[2 * idx.texcoord_index + 1]
					);
				}
				else {
					vertex.UV = MirageEngine::Core::Math::Vector2(0, 0);
				}

				uint32_t vertexIndex = static_cast<uint32_t>(_vertices.size());
				_vertices.push_back(vertex);

				faceIndices.push_back(vertexIndex);
			}

			// Triangulate faces if necessary
			if (fv == 3) {
				indices.push_back(faceIndices[0]);
				indices.push_back(faceIndices[1]);
				indices.push_back(faceIndices[2]);
			}
			else if (fv == 4) {
				indices.push_back(faceIndices[0]);
				indices.push_back(faceIndices[1]);
				indices.push_back(faceIndices[2]);

				indices.push_back(faceIndices[0]);
				indices.push_back(faceIndices[2]);
				indices.push_back(faceIndices[3]);
			}
			else {
				for (size_t v = 1; v < fv - 1; v++) {
					indices.push_back(faceIndices[0]);
					indices.push_back(faceIndices[v]);
					indices.push_back(faceIndices[v + 1]);
				}
			}

			index_offset += fv;
		}
	}
}

MirageEngine::Core::Resources::ResourceManager::~ResourceManager()
{
	for (auto& resource : m_resources) 
		delete resource.second.release();
	for (auto& shader : m_shaderCache) 
		shader.second.reset();
	m_resources.clear();
	m_shaderCache.clear();

}