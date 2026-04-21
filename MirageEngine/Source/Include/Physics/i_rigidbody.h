#pragma once

#include <iostream>
#include "common_enum.h"
#include <Components/component.h>
#include <GameObject/game_object.h>
#include <vector3.h>

namespace MirageEngine::Core::Physics
{
	class Bullet3RigidBody;
	class ICollider;

	class IRigidBody : public MirageEngine::Core::Components::Component
	{
	protected:
		using Vector3 = MirageEngine::Core::Math::Vector3;

		float          m_Mass = 1.f;
		Vector3        m_Velocity = Vector3(0.f);
		ERigidBodyType m_Type;
		ECollisionGroup		m_CollisionGroup = ECollisionGroup::Default;
		ECollisionGroup		m_CollisionMask = ECollisionGroup::All;
		ECollisionResponse m_CollisionRespond = ECollisionResponse::Block;
		bool m_bIsDirty = true;

		std::vector<ICollider*> m_CurrentRigidBodyShapesBind;

	public:

		IRigidBody() = default;
		virtual ~IRigidBody() = default;

		virtual bool Create(ICollider* _shape, ERigidBodyType _type) = 0;

		virtual void OnTransformChanged(MirageEngine::Core::Components::Transform* _newTransform) = 0;
		virtual void SetType(ERigidBodyType _type) = 0;

		virtual void AddCollider(ICollider* _newCollider) = 0;
		virtual void RemoveCollider(ICollider* _collider) = 0;

		virtual void SetMass(float _mass) = 0;
		void SetCollisionGroup(ECollisionGroup _group)
		{
			m_CollisionGroup = _group;
			m_bIsDirty = true;
		}
		void SetCollisionMask(ECollisionGroup _mask)
		{
			m_CollisionMask = _mask;
			m_bIsDirty = true;
		}

		void SetCollisionResponse(ECollisionResponse response)
		{
			m_CollisionRespond = response;
			m_bIsDirty = true;
		}

		ECollisionGroup GetCollisionGroup() const { return m_CollisionGroup; }
		ECollisionGroup GetCollisionMask() const { return m_CollisionMask; }
		entt::id_type GetType() const override { return 0; }


		virtual Bullet3RigidBody& API_Bullet3() { throw std::runtime_error("Bad API Call : object is not a Bullet3RigidBody"); }
	};
} // namespace MirageEngine::Core::Physics
