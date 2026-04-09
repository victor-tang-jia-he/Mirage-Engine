#pragma once
#include <quaternion.h>
#include <Components/component.h>

namespace MirageEngine
{
	namespace Core
	{
		namespace Components
		{
			class Transform
			{
			private:
				Math::Vector3 m_LocalPosition{ 0.f,0.f,0.f };
				Math::Vector3 m_LocalScale{ 1.f,1.f,1.f };
				//Math::Quaternion m_LocalRotation{ 0.f,0.f,0.f };

				Math::Quaternion m_LocalRotation = Math::Quaternion::Identity();
				Math::Matrix4 m_LocalMatrix = Math::Matrix4::Identity();
				Math::Matrix4 m_WorldMatrix = Math::Matrix4::Identity();

				bool m_bDirty = true;

				// utility
				Math::Vector3 Rotate(const Math::Quaternion& q, const Math::Vector3& v) const;

			public:
				Transform() = default;
				Transform(Math::Vector3 _position, Math::Quaternion _rotation, Math::Vector3 _scale);
				void Init();
				void Update();

				void LookAt(const Math::Vector3& _target);

				Math::Vector3 Forward() const;
				Math::Vector3 Up() const;
				Math::Vector3 Right() const;
				Math::Vector3 Left() const;

				// Getters
				Math::Matrix4 GetMatrix();
				Math::Vector3 GetPosition() const { return m_LocalPosition; }
				Math::Vector3 GetScale() const { return m_LocalScale; }
				Math::Quaternion GetRotation()  const { return m_LocalRotation; }
				Math::Vector3 GetRotationEuler() const { return m_LocalRotation.ToEulerAngle() ; }
				const MirageEngine::Core::Math::Matrix4& GetWorldMatrix() const { return m_WorldMatrix; }
				bool GetIsDirty() const { return m_bDirty; }

				// Setters
				void SetPosition(Math::Vector3 _pos);
				void SetScale(Math::Vector3 _scale);
				void SetRotation(Math::Quaternion _rotation);
				void SetWorldMatrix(const Math::Matrix4& _world) { m_WorldMatrix = _world; m_bDirty = false; }
				void SetDirty(bool _state) { m_bDirty = _state; }
				void SetLocalMatrix(Math::Matrix4 _mat);
			};
		}
	}
}