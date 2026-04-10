#pragma once

#include "bullet3_pch.h"
#include "../i_collider.h"

namespace MirageEngine::Core::Physics
{
    class Bullet3SphereCollider : public ICollider, public IBulletColliderData, public ISphereCollider
    {
            using Vector3    = MirageEngine::Core::Math::Vector3;
            using Quaternion = MirageEngine::Core::Math::Quaternion;

        private:

            btCollisionShape* m_CollisionShape = nullptr;
            float             m_Radius         = 1.f;
            btTransform       m_LocalTransform;

        public:

            Bullet3SphereCollider() = default;
            ~Bullet3SphereCollider() override;

            void Create() override;

            void Init() override {};
            void Update() override {};

            void SetOffset(Vector3 _offset, Quaternion _rotation) override;

            void SetRadius(float _radius) override;

            float GetRadius() const override { return m_Radius; }

            btCollisionShape* GetShape() override { return m_CollisionShape; }

            IBulletColliderData* AsBullet3Collider() override { return this; }

            btTransform GetTransform() override;
    };
} // namespace MirageEngine::Core::Physics
