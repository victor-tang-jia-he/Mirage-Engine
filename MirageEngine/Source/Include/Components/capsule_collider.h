#pragma once

#include <Components/physics_components_interface.h>
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
        } // namespace Math

        namespace Physics
        {
            class ICollider;
        }

        namespace Components
        {
            class CapsuleCollider : public Component, public ICCollider
            {
                    Physics::ICollider* m_Collider;

                public:

                    CapsuleCollider();
                    ~CapsuleCollider();

                    virtual void Init() override {}

                    virtual void Update() override {}

                    entt::id_type GetType() const override { return 0; }

                    // Capsule only parameter
                    void  SetRadius(float _radius);
                    void  SetHeight(float _height);
                    float GetRadius() const;
                    float GetHeight() const;

                    // Global collider parameter
                    void SetScale(Math::Vector3 _scale);
                    void SetTranslate(Math::Vector3 _offset);
                    void SetRotation(Math::Quaternion _rotation);
                    void SetOffset(Math::Vector3 _offset, Math::Quaternion _rotation);

                    Math::Vector3    GetScale() const;
                    Math::Vector3    GetTranslate() const;
                    Math::Quaternion GetRotation() const;

                    Physics::ICollider* GetCollision() const override { return m_Collider; }
            };
        } // namespace Components
    } // namespace Core
} // namespace MirageEngine
