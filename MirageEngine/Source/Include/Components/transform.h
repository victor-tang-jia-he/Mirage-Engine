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
				MirageEngine::Core::Math::Vector3 m_LocalPosition{ 0.f,0.f,0.f };
				MirageEngine::Core::Math::Vector3 m_LocalScale{ 1.f,1.f,1.f };
				MirageEngine::Core::Math::Quaternion m_LocalRotation{ 0.f,0.f,0.f };

				MirageEngine::Core::Math::Matrix4 m_LocalMatrix;
				MirageEngine::Core::Math::Matrix4 m_WorldMatrix;

				bool m_bDirty = true;

				// utility
				MirageEngine::Core::Math::Vector3 Rotate(const MirageEngine::Core::Math::Quaternion& q, const MirageEngine::Core::Math::Vector3& v) const;

			public:
				Transform() = default;
				Transform(MirageEngine::Core::Math::Vector3 _position, MirageEngine::Core::Math::Quaternion _rotation, MirageEngine::Core::Math::Vector3 _scale);

				void Init();
				void Update();

				void LookAt(const MirageEngine::Core::Math::Vector3& _target);

				MirageEngine::Core::Math::Vector3 Forward() const;
				MirageEngine::Core::Math::Vector3 Up() const;
				MirageEngine::Core::Math::Vector3 Right() const;
				MirageEngine::Core::Math::Vector3 Left() const;

				void SetPosition(MirageEngine::Core::Math::Vector3 _pos);	
				void SetScale(MirageEngine::Core::Math::Vector3 _scale);
				void SetRotation(MirageEngine::Core::Math::Quaternion _rotation);
				void SetWorldMatrix(const MirageEngine::Core::Math::Matrix4& _world) { m_WorldMatrix = _world; m_bDirty = false; }
				void SetDirty(bool _state) { m_bDirty = _state; }

				MirageEngine::Core::Math::Matrix4 GetMatrix();
				MirageEngine::Core::Math::Vector3 GetPosition() const { return m_LocalPosition; }
				MirageEngine::Core::Math::Vector3 GetScale() const { return m_LocalScale; }
				MirageEngine::Core::Math::Quaternion GetRotation()  const { return m_LocalRotation; }
				MirageEngine::Core::Math::Vector3 GetRotationEuler() const { return m_LocalRotation.ToEulerAngle() ; }
				const MirageEngine::Core::Math::Matrix4& GetWorldMatrix() const { return m_WorldMatrix; }
				bool IsDirty() const { return m_bDirty; }
			};
		}
	}
}