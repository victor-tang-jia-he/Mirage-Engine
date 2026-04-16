#pragma once

#include <Components/component.h>

namespace MirageEngine
{
	class GameObject;
	namespace Core
	{
		namespace Math
		{
			struct Quaternion;
			struct Vector3;
		}
		namespace Physics
		{
			class ICollider;
		}
		namespace Components
		{
			class BoxCollider : public Component
			{
				std::shared_ptr<Physics::ICollider> m_Collider;

			public:
				BoxCollider();
				~BoxCollider();
				virtual void Init() override {}
				virtual void Update() override {}

				entt::id_type GetType() const override {
					return 0;
				}

				void SetOffset(MirageEngine::Core::Math::Vector3 _offset, MirageEngine::Core::Math::Quaternion _rotation);
				void SetHalfExtents(const MirageEngine::Core::Math::Vector3& _halfExtents);

				Physics::ICollider* GetCollision() const { return m_Collider.get(); }
			};
		}
	}
}