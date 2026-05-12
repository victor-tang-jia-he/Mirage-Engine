#pragma once

#include <iostream>
#include "common_enum.h"
#include <GameObject/game_object.h>
#include <vector3.h>
namespace MirageEngine::Core::Components {
    class Transform;
}
namespace MirageEngine::Core::Physics
{
    class Bullet3RigidBody;
    class ICollider;

    class IRigidBody
    {
        protected:

            using Vector3 = MirageEngine::Core::Math::Vector3;
            using Transform = MirageEngine::Core::Components::Transform;

            float m_Mass = 1.f;
            Vector3 m_Velocity = Vector3(0.f);
            ERigidBodyType m_Type;
            ECollisionGroup m_CollisionGroup = ECollisionGroup::Default;
            ECollisionGroup m_CollisionMask = ECollisionGroup::All;
            ECollisionResponse m_CollisionRespond = ECollisionResponse::Block;
            bool m_bIsDirty = true;

            std::vector<ICollider*> m_CurrentRigidBodyShapesBind;

        public:
            
            std::function<void()> OnSimulated;

            IRigidBody() = default;
            virtual ~IRigidBody() = default;

            virtual bool Create(std::shared_ptr<ICollider> _shape, ERigidBodyType _type, MirageEngine::Core::Components::Transform& _newTransform) = 0;

            virtual void UpdateTransform(MirageEngine::Core::Components::Transform& _transform) = 0;

            virtual void OnTransformChanged(MirageEngine::Core::Components::Transform& _newTransform) = 0;
            virtual void SetType(ERigidBodyType _type) = 0;

            virtual void AddCollider(std::shared_ptr<ICollider> _newCollider) = 0;
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

            ERigidBodyType GetType() const { return m_Type; }

            float GetMass() const { return m_Mass; }

            ECollisionGroup GetCollisionGroup() const { return m_CollisionGroup; }

            ECollisionGroup GetCollisionMask() const { return m_CollisionMask; }

            ECollisionResponse GetCollisionResponse() const{ return m_CollisionRespond; }

            virtual Bullet3RigidBody& API_Bullet3() { throw std::runtime_error("Bad API Call : object is not a Bullet3RigidBody"); }
    };
} // namespace MirageEngine::Core::Physics
