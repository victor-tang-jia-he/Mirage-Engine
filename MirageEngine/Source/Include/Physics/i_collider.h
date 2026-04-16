// i_collider.h
#pragma once

#include <iostream>
#include "common_enum.h"
#include <Components/component.h>
#include <Components/transform.h>

// Forward declarations
class btCollisionShape;
class btTransform;

namespace MirageEngine::Core::Physics
{
	class IRigidBody;
	using Vector3 = MirageEngine::Core::Math::Vector3;
	using Quaternion = MirageEngine::Core::Math::Quaternion;

	//=========================================================================
	// Collider-specific interfaces
	//=========================================================================
	class ISphereCollider
	{
	public:

		virtual ~ISphereCollider() = default;
		virtual void  SetRadius(float _radius) = 0;
		virtual float GetRadius() const = 0;
	};

	class IBoxCollider
	{
	public:

		virtual ~IBoxCollider() = default;
		virtual void    SetHalfExtents(const Vector3& _halfExtents) = 0;
		virtual Vector3 GetHalfExtents() const = 0;
	};

	class ICapsuleCollider
	{
	public:

		virtual ~ICapsuleCollider() = default;
		virtual void  SetRadius(float _radius) = 0;
		virtual void  SetHeight(float _height) = 0;
		virtual float GetRadius() const = 0;
		virtual float GetHeight() const = 0;
	};

	class IBulletColliderData
	{
	public:

		virtual ~IBulletColliderData() = default;
		virtual btCollisionShape* GetShape() = 0;
		virtual btTransform       GetTransform() = 0;
	};

	//=========================================================================
	// ICollider - Base collider interface
	//=========================================================================
	class ICollider : public MirageEngine::Core::Components::Component
	{
	protected:

		Vector3             m_Scale = Vector3(1.f);
		Vector3             m_Offset = Vector3(0.f);
		Quaternion          m_Rot = Quaternion(0.f, 0.f, 0.f, 1.f);
		ECollisionShapeType m_Type;

		IRigidBody* m_RigidBody = nullptr;

		bool m_bIsDirty = true;

	public:

		virtual ~ICollider() = default;

		virtual void Create() = 0;
		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void SetOffset(Vector3 _offset, Quaternion _rotation) = 0;

		bool SetRigidbody(IRigidBody* _rigidbody);
		void SetScale(Vector3 _scale)
		{
			m_Scale = _scale;
			m_bIsDirty = true;
		}
		void SetTranslate(Vector3 _offset)
		{
			m_Offset = _offset;
			m_bIsDirty = true;
		}
		void SetRotation(Quaternion _rotation)
		{
			m_Rot = _rotation;
			m_bIsDirty = true;
		}
		Vector3 GetScale() const { return m_Scale; }
		Vector3 GetTranslate() const { return m_Offset; }
		Quaternion GetRotation() const { return m_Rot; }
		bool GetIsDirty() const { return m_bIsDirty; }
		entt::id_type GetType() const override { return 0; }
		ECollisionShapeType GetColliderType() const { return m_Type; }

		virtual IBulletColliderData* AsBullet3Collider() { throw std::runtime_error("Bad API Call : object is not an Bullet3 Collider"); }

		template <typename T> T* As() { return dynamic_cast<T*>(this); }
	};
} // namespace MirageEngine::Core::Physics
