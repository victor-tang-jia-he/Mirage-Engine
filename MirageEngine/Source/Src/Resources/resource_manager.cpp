#include <Resources/resource_manager.h>
#include <RHI/OpenGL/openGL_buffer.h>
#include <RHI/OpenGL/openGL_pipeline.h>
#include <RHI/OpenGL/openGL_pipeline_parameters.h>
#include <RHI/OpenGL/openGL_shader.h>
#include <RHI/i_texture.h>
#include <RHI/i_render_interface.h>

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

MirageRHI::ITexture* ResourceManager::CreateTexture(const std::string&, const std::string&)
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
	m_BufferVertices->Create(vertices.data(), vertices.size() * sizeof(MirageRHI::LitVertex), MirageRHI::IBuffer::BufferType::Vertices);
	m_BufferIndices->Create(indices.data(), indices.size() * sizeof(MirageRHI::LitVertex), MirageRHI::IBuffer::BufferType::Indices);
	IMesh* mesh = new IMesh(m_BufferVertices, m_BufferIndices, indices.size());
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

IMesh::IMesh(MirageRHI::IBuffer* _vert, MirageRHI::IBuffer* _indiceBuffer, size_t _indicesCount)
	: m_vertexBuffer(_vert), m_indicesBuffer(_indiceBuffer), m_indicesCount(_indicesCount)
{
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

	// Pour suivre les vertices d¨¦j¨¤ ajout¨¦s (optionnel, pour d¨¦duplication)
	std::unordered_map<size_t, uint32_t> vertexMap;

	// Loop over shapes
	for (const auto& shape : shapes) {
		size_t index_offset = 0;
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
			size_t fv = shape.mesh.num_face_vertices[f];

			// Pour les triangles, on doit cr¨¦er des indices corrects
			std::vector<uint32_t> faceIndices;

			// Loop over vertices in the face
			for (size_t v = 0; v < fv; v++) {
				tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

				// Calculer une cl¨¦ unique pour ce vertex (position+normal+uv)
				size_t vertexKey = idx.vertex_index;
				if (idx.normal_index >= 0) vertexKey = vertexKey * 1000000 + idx.normal_index;
				if (idx.texcoord_index >= 0) vertexKey = vertexKey * 1000000 + idx.texcoord_index;

				uint32_t vertexIndex;
				auto it = vertexMap.find(vertexKey);

				if (it != vertexMap.end()) {
					// Vertex d¨¦j¨¤ existant
					vertexIndex = it->second;
				}
				else {
					// Cr¨¦er un nouveau vertex
					MirageRHI::LitVertex vertex;

					// Position
					vertex.Position = MirageEngine::Core::Math::Vector3(
						attrib.vertices[3 * idx.vertex_index + 0],
						attrib.vertices[3 * idx.vertex_index + 1],
						attrib.vertices[3 * idx.vertex_index + 2]
					);

					// Normal (si disponible)
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

					// UV (si disponible)
					if (idx.texcoord_index >= 0) {
						vertex.UV = MirageEngine::Core::Math::Vector2(
							attrib.texcoords[2 * idx.texcoord_index + 0],
							attrib.texcoords[2 * idx.texcoord_index + 1]
						);
					}
					else {
						vertex.UV = MirageEngine::Core::Math::Vector2(0, 0);
					}

					vertexIndex = static_cast<uint32_t>(_vertices.size());
					_vertices.push_back(vertex);
					vertexMap[vertexKey] = vertexIndex;
				}

				faceIndices.push_back(vertexIndex);
			}

			// Convertir les faces en triangles (si la face a plus de 3 vertices)
			if (fv == 3) {
				// Face triangulaire
				indices.push_back(faceIndices[0]);
				indices.push_back(faceIndices[1]);
				indices.push_back(faceIndices[2]);
			}
			else if (fv == 4) {
				// Face quad -> deux triangles
				indices.push_back(faceIndices[0]);
				indices.push_back(faceIndices[1]);
				indices.push_back(faceIndices[2]);

				indices.push_back(faceIndices[0]);
				indices.push_back(faceIndices[2]);
				indices.push_back(faceIndices[3]);
			}
			else {
				// Fan triangulation pour les polygones avec plus de 4 vertices
				for (size_t v = 1; v < fv - 1; v++) {
					indices.push_back(faceIndices[0]);
					indices.push_back(faceIndices[v]);
					indices.push_back(faceIndices[v + 1]);
				}
			}

			index_offset += fv;
		}
	}

	std::cout << "Loaded " << _vertices.size() << " vertices, "
		<< indices.size() << " indices from " << _path << std::endl;
}