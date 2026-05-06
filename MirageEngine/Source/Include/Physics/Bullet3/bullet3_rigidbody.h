#pragma once
#include "bullet3_pch.h"
#include "../i_rigidbody.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBullet3Rigidbody)

namespace MirageEngine::Core::Physics
{
    class Bullet3BoxCollider;

    class Bullet3RigidBody : public IRigidBody, public std::enable_shared_from_this<Bullet3RigidBody>
    {
            friend class Bullet3BoxCollider;
            using Transform = MirageEngine::Core::Components::Transform;

        private:

            btRigidBody*                     m_rigidBody    = nullptr;
            std::shared_ptr<btCompoundShape> m_Compound     = nullptr;
            Vector3                          m_CurrentScale = { 1, 1, 1 };

        public:

            Bullet3RigidBody() = default;
            ~Bullet3RigidBody() override;

            bool Create(std::shared_ptr<ICollider> _shape, ERigidBodyType _type) override;

            void UpdateTransform(Transform& _transform) override;
            void OnTransformChanged(Transform& _newTransform) override;
            void SetType(ERigidBodyType _type) override;
            void SetMass(float _mass) override;

            void SetScale(const Vector3& scale);

            void AddCollider(std::shared_ptr<ICollider> _newCollider) override;
            void RemoveCollider(ICollider* _collider) override;

            void UpdateCompound();

            btRigidBody* GetBullet3RigidBody() const { return m_rigidBody; }

            Bullet3RigidBody& API_Bullet3() override { return *this; }
    };
} // namespace MirageEngine::Core::Physics
