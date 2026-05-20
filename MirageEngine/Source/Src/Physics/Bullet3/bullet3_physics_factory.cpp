#include <Physics/Bullet3/bullet3_physics_factory.h>
#include <Physics/i_physics_factory.h>
#include <Physics/Bullet3/bullet3_rigidbody.h>
#include <Physics/Bullet3/bullet3_box_collider.h>
#include <Physics/Bullet3/bullet3_sphere_collider.h>
#include <Physics/Bullet3/bullet3_capsule_collider.h>
#include <Physics/Bullet3/bullet3_world_physics.h>

#include <Logs/log.h>

DEFINE_LOG_CATEGORY(LogBullet3PhysicsFactory)

namespace MirageEngine::Core::Physics
{
	std::shared_ptr<IRigidBody> Bullet3PhysicsFactory::CreateRigidBody()
	{
		return std::make_shared<Bullet3RigidBody>();
	}

	std::shared_ptr<ICollider> Bullet3PhysicsFactory::CreateCollider(ECollisionShapeType _shapeType)
	{
		switch (_shapeType)
		{
		case ECollisionShapeType::BOX:
			return std::make_shared<Bullet3BoxCollider>();
		case ECollisionShapeType::SPHERE:
			return std::make_shared<Bullet3SphereCollider>();
		case ECollisionShapeType::CAPSULE:
			return std::make_shared<Bullet3CapsuleCollider>();
		default:
			LOG(logWarning, LogBullet3PhysicsFactory , "Unsupported collider type requested: ", static_cast<int>(_shapeType));
			return nullptr;
		}
	}

	std::shared_ptr<IWorldPhysics> Bullet3PhysicsFactory::CreateWorld()
	{
		return std::make_shared<Bullet3WorldPhysics>();
	}
} // namespace MirageEngine::Core::Physics