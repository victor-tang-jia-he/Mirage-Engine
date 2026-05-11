#include <Components/mesh_renderer.h>

#include <GameObject/game_object.h>

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
