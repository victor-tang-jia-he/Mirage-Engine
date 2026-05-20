#pragma once

#include <Physics/i_physics_factory.h>

DECLARE_LOG_CATEGORY_EXTERN(LogBullet3PhysicsFactory)

namespace MirageEngine::Core::Physics
{
	class Bullet3PhysicsFactory : public IPhysicsFactory
	{
	public:
		virtual std::shared_ptr<IRigidBody> CreateRigidBody() override;
		virtual std::shared_ptr<ICollider> CreateCollider(ECollisionShapeType _shapeType) override;
		virtual std::shared_ptr<IWorldPhysics> CreateWorld() override;
	};
} // namespace MirageEngine::Core::Physics