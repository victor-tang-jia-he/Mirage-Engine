#pragma once

#include "bullet3_pch.h"
#include "../i_collider.h"

namespace MirageEngine::Core::Physics
{
    class Bullet3SphereCollider : public ICollider, public IBulletColliderData
    {
            using Vector3    = MirageEngine::Core::Math::Vector3;
            using Quaternion = MirageEngine::Core::Math::Quaternion;

        private:

            btCollisionShape* m_CollisionShape = nullptr;
            btCompoundShape*  m_Compound       = nullptr;
            float             m_Radius         = 1.f;

        public:

            Bullet3SphereCollider() = default;
            ~Bullet3SphereCollider() override;

            bool Create() override;

            void Init() override {};
            void Update() override {};

            void SetOffset(Vector3 _offset, Quaternion _rotation) override;

            btCollisionShape* GetShape() override { return m_CollisionShape; }

            IBulletColliderData* AsBullet3Collider() override { return this; }
    };
} // namespace MirageEngine::Core::Physics
