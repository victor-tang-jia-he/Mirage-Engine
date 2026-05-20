#pragma once

#include <Physics/common_enum.h>

// Maths
#include <matrix4.h>

namespace MirageEngine::Core::Components
{
    class Transform;
}

namespace MirageEngine::Core
{
    namespace Physics
    {
        class Bullet3RigidBody;
        class ICollider;
        class IRigidBody
        {
        protected:
            using Vector3 = MirageEngine::Core::Math::Vector3;
            using Vector4 = MirageEngine::Core::Math::Vector4;
            using Matrix4 = MirageEngine::Core::Math::Matrix4;
            using Transform = Components::Transform;

            float m_Mass = 1.f;
            Vector3 m_Velocity;
            ERigidBodyType m_Type;
            ECollisionGroup m_CollisionGroup = ECollisionGroup::GROUP_1;
            ECollisionGroup m_CollisionMask = ECollisionGroup::ALL;
            ECollisionResponse m_CollisionRespond = ECollisionResponse::BLOCK;
            bool m_bIsDirty = true;

            std::vector<std::weak_ptr<ICollider>> m_CurrentRigidBodyShapesBind;

        public:
            std::function<void()> OnSimulated;

            IRigidBody() = default;
            virtual ~IRigidBody() = default;

            virtual bool Create(std::shared_ptr<ICollider> _shape, ERigidBodyType _type, Components::Transform& _newTransform) = 0;

            virtual void UpdateTransform(Components::Transform& _transform) = 0;

            virtual void OnTransformChanged(Components::Transform& _newTransform) = 0;
            virtual void SetType(ERigidBodyType _type) = 0;

            virtual void AddCollider(std::shared_ptr<ICollider> _newCollider) = 0;
            virtual void RemoveCollider(std::shared_ptr<ICollider> _collider) = 0;

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

            ERigidBodyType GetType() const { return m_Type; }
            float GetMass() const { return m_Mass; }
            ECollisionGroup GetCollisionGroup() const { return m_CollisionGroup; }
            ECollisionGroup GetCollisionMask() const { return m_CollisionMask; }
            ECollisionResponse GetCollisionResponse() const { return m_CollisionRespond; }

            template <typename T> T* As() { return dynamic_cast<T*>(this); }
        };
    }
} // namespace MirageEngine::Core::Physics
