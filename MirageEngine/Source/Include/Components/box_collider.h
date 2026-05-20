#pragma once

#include <Components/physics_components_interface.h>
#include <Scripting/engine_script_api.h>

namespace MirageEngine
{
    class GameObject;

    namespace Core
    {
        namespace Math
        {
            struct Quaternion;
            struct Vector3;
        } // namespace Math

        namespace Physics
        {
            class ICollider;
		} // namespace Physics

        namespace Components
        {
            class ENGINE_API BoxCollider : public Component, public ICCollider
            {
			private:
                std::shared_ptr<Physics::ICollider> m_Collider;

            public:
                BoxCollider();
                BoxCollider(GameObject* _owner);
                ~BoxCollider() = default;

                void AttachMeta(std::shared_ptr<GameObject> _Owner);
                static void AttachNew(std::shared_ptr<GameObject> _owner);

                entt::id_type GetType() const override;

                // Box only parameter
                void SetHalfExtents(const Math::Vector3& _halfExtents);
                Math::Vector3 GetHalfExtents();

                // Global collider parameter
                void SetScale(Math::Vector3 _scale);
                void SetTranslate(Math::Vector3 _offset);
                void SetRotation(Math::Quaternion _rotation);
                void SetOffset(Math::Vector3 _offset, Math::Quaternion _rotation);

                Math::Vector3 GetScale() const;
                Math::Vector3 GetTranslate() const;
                Math::Quaternion GetRotation() const;

                std::shared_ptr<Physics::ICollider> GetCollision() const override { return m_Collider; }
            };
        } // namespace Components
    } // namespace Core
} // namespace MirageEngine
