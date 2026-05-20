#include <Components/mesh_renderer.h>

#include <entt/entt.hpp>
#include <GameObject/game_object.h>
#include <Resources/resource_manager.h>

MirageEngine::Core::Components::MeshRenderer::MeshRenderer()
{
	using namespace MirageEngine::Core::Resources;
	m_Mesh = ResourceManager::Instance().GetMesh("cube");
	m_Material = ResourceManager::Instance().GetMaterialInstance("DefaultMaterialInstance");
}

MirageEngine::Core::Components::MeshRenderer::MeshRenderer(GameObject* _owner)
{
	using namespace MirageEngine::Core::Resources;
	m_Mesh = ResourceManager::Instance().GetMesh("cube");
	m_Material = ResourceManager::Instance().GetMaterialInstance("DefaultMaterialInstance");
}

void MirageEngine::Core::Components::MeshRenderer::AttachMeta(std::shared_ptr<GameObject> _Owner)
{
	std::shared_ptr<MeshRenderer> compPtr = std::make_shared<MeshRenderer>(*this);
	_Owner->AddComponent<MeshRenderer>(compPtr);
}

void MirageEngine::Core::Components::MeshRenderer::AttachNew(std::shared_ptr<GameObject> _owner)
{
	std::shared_ptr<MeshRenderer> compPtr = std::make_shared<MeshRenderer>();
	_owner->AddComponent<MeshRenderer>(compPtr);
}

entt::id_type MirageEngine::Core::Components::MeshRenderer::GetType() const
{
	return entt::type_id<MeshRenderer>().hash();
}
