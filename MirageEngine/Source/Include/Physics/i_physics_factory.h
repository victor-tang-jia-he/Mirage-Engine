#pragma once

#include <Physics/common_enum.h>

namespace MirageEngine::Core::Physics
{
	class IRigidBody;
	class ICollider;
	class IWorldPhysics;

	class IPhysicsFactory 
	{
	public:
		virtual std::shared_ptr<IRigidBody>  CreateRigidBody() = 0;
		virtual std::shared_ptr<ICollider>   CreateCollider(ECollisionShapeType) = 0;
		virtual std::shared_ptr<IWorldPhysics> CreateWorld() = 0;
	};
}