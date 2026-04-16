#include <Components/box_collider.h>
#include <quaternion.h>
#include <Physics/physics_manager.h>

using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Physics;
using namespace MirageEngine::Core;

MirageEngine::Core::Components::BoxCollider::BoxCollider()
{
	m_Collider = PhysicsManager::Instance().CreateCollider(ECollisionShapeType::Box);
}

MirageEngine::Core::Components::BoxCollider::~BoxCollider()
{
	m_Collider.reset();
}

void BoxCollider::SetOffset(MirageEngine::Core::Math::Vector3 _offset, MirageEngine::Core::Math::Quaternion _rotation)
{
	if (m_Collider) m_Collider->As<BoxCollider>()->SetOffset(_offset, _rotation);
}
void BoxCollider::SetHalfExtents(const MirageEngine::Core::Math::Vector3& _halfExtents)
{
	if (m_Collider) m_Collider->As<BoxCollider>()->SetHalfExtents(_halfExtents);
}