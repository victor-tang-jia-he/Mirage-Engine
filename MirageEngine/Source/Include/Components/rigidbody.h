#pragma once

#include <Components/component.h>
#include <Components/physics_components_interface.h>

DECLARE_LOG_CATEGORY_EXTERN(LogComponentRigidBody)

namespace MirageEngine
{
    class GameObject;

    namespace Core
    {
        namespace Components
        {
            class Component;
            class Transform;
        } // namespace Components

        namespace Math
        {
            struct Quaternion;
            struct Vector3;
        } // namespace Math

        namespace Physics
        {
            class IRigidBody;
            enum class ERigidBodyType;
            enum class ECollisionGroup;
            enum class ECollisionResponse;
        } // namespace Physics

        namespace Components
        {
            class Rigidbody : public Components::Component
            {
                    Physics::IRigidBody* m_Rigidbody;

                public:

                    Rigidbody(ICCollider* _shape, Physics::ERigidBodyType _type);
                    ~Rigidbody();

                    entt::id_type GetType() const override { return 0; }

                    void OnTransformChanged(Components::Transform& _newTransform);
                    void SetType(Physics::ERigidBodyType _type);

                    void AddCollider(ICCollider* _newCollider);
                    void RemoveCollider(ICCollider* _collider);

                    void SetMass(float _mass);

                    void SetCollisionGroup(Physics::ECollisionGroup _group);

                    void SetCollisionMask(Physics::ECollisionGroup _mask);

                    void SetCollisionResponse(Physics::ECollisionResponse response);

                    Physics::ECollisionGroup GetCollisionGroup() const;

                    Physics::ECollisionGroup GetCollisionMask() const;
            };
        } // namespace Components
    } // namespace Core
} // namespace MirageEngine
