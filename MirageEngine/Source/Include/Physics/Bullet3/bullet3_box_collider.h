#pragma once

#include "bullet3_pch.h"
#include "../i_collider.h"

namespace MirageEngine::Core::Physics
{
    class Bullet3RigidBody;

    class Bullet3BoxCollider : public ICollider, public IBulletColliderData, public IBoxCollider
    {
            using Vector3    = MirageEngine::Core::Math::Vector3;
            using Quaternion = MirageEngine::Core::Math::Quaternion;

        private:

            btCollisionShape* m_CollisionShape = nullptr;
            Vector3           m_HalfExtents    = Vector3(1.f);
            btTransform       m_LocalTransform;

        public:

            Bullet3BoxCollider() = default;
            ~Bullet3BoxCollider() override;

            void Create() override;
            void Init() override {};
            void Update() override {};

            void SetOffset(Vector3 _offset, Quaternion _rotation) override;
            void SetHalfExtents(const Vector3& _halfExtents) override;

            Vector3     GetHalfExtents() const override;
            btTransform GetTransform() override;

            btCollisionShape* GetShape() override { return m_CollisionShape; }

            IBulletColliderData* AsBullet3Collider() override { return this; }
    };
} // namespace MirageEngine::Core::Physics
