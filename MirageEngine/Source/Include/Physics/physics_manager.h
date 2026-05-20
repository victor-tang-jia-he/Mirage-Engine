#pragma once

#include <Physics/common_enum.h>

#include <Scripting/engine_script_api.h>

#include <Components/transform.h>

DECLARE_LOG_CATEGORY_EXTERN(LogPhysics);

namespace MirageEngine::Core::Physics
{
	class IWorldPhysics;
	class ICollider;
	class IRigidBody;
	class IPhysicsFactory;

	class ENGINE_API PhysicsManager
	{
	private:
		EPhysicsEngine m_PhysicsEngine = EPhysicsEngine::BULLET_3;
		std::vector<IRigidBody*> m_WorldRigidbodys;
		std::shared_ptr<IWorldPhysics> m_WorldPhysics = nullptr;
		std::shared_ptr<IPhysicsFactory> m_PhysicsFactory = nullptr;

	public:
		static PhysicsManager& Instance();

		void Shutdown();

		~PhysicsManager() = default;
		void Initialize(EPhysicsEngine _physicsEngine = EPhysicsEngine::BULLET_3);
		void Update(float _deltaTime);
		void DebugDraw();

		std::weak_ptr<IWorldPhysics> GetWorldPhysics() const { return m_WorldPhysics; }

		std::shared_ptr<IRigidBody> CreateRigidBody(std::shared_ptr<ICollider> _shape, ERigidBodyType _type, Components::Transform _newTransform = Components::Transform());

		std::shared_ptr<ICollider> CreateCollider(ECollisionShapeType _shapeType);

		void RegisterRigidBody(std::shared_ptr<IRigidBody> _body, ECollisionGroup _group, ECollisionGroup _mask);

		void UnregisterRigidBody(IRigidBody* _body);
	private:
		PhysicsManager() = default;
	};
} // namespace MirageEngine::Core::Physics
