#pragma once

#include "bullet3_pch.h"
#include "../i_collider.h"

namespace MirageEngine::Core::Physics
{
    class Bullet3CapsuleCollider : public ICollider, public IBulletColliderData, public ICapsuleCollider
    {
            using Vector3    = MirageEngine::Core::Math::Vector3;
            using Quaternion = MirageEngine::Core::Math::Quaternion;

        private:

            btCollisionShape* m_CollisionShape = nullptr;
            btCompoundShape*  m_Compound       = nullptr;
            float             m_Radius         = 1.f;
            float             m_Height         = 2.f;

        public:

            Bullet3CapsuleCollider() = default;
            ~Bullet3CapsuleCollider() override;

            bool Create() override;

            void Init() override {};
            void Update() override {};

            void SetOffset(Vector3 _offset, Quaternion _rotation) override;

            void SetRadius(float _radius) override;
            void SetHeight(float _height) override;

            float GetRadius() const override { return m_Radius; }
            float GetHeight() const override { return m_Height; }

            btCollisionShape* GetShape() override { return m_CollisionShape; }

            IBulletColliderData* AsBullet3Collider() override { return this; }

        private:

            void UpdateRigidBodyTransform();

            void UpdateCompoundShape();

            void UpdateCollisionShape();
    };
} // namespace MirageEngine::Core::Physics
