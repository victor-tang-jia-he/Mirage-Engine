#pragma once
#include <Physics/Bullet3/bullet3_pch.h>
#include <Physics/i_rigidbody.h>

DECLARE_LOG_CATEGORY_EXTERN(LogBullet3Rigidbody)

namespace MirageEngine::Core::Physics
{
    class Bullet3RigidBody : public IRigidBody, public std::enable_shared_from_this<Bullet3RigidBody>
    {
        using Transform = Components::Transform;

    private:
        std::shared_ptr<btRigidBody> m_RigidBody = nullptr;
        std::shared_ptr<btCompoundShape> m_Compound = nullptr;
        Vector3 m_CurrentScale = { 1, 1, 1 };

    public:
        Bullet3RigidBody() = default;
        ~Bullet3RigidBody() override;

        bool Create(std::shared_ptr<ICollider> _shape, ERigidBodyType _type, Components::Transform& _newTransform) override;

        void UpdateTransform(Transform& _transform) override;
        void OnTransformChanged(Transform& _newTransform) override;
        void SetType(ERigidBodyType _type) override;
        void SetMass(float _mass) override;

        void SetScale(const Vector3& scale);

        void AddCollider(std::shared_ptr<ICollider> _newCollider) override;
        void RemoveCollider(std::shared_ptr<ICollider> _collider) override;

        void UpdateCompound();

        std::shared_ptr<btRigidBody> GetBullet3RigidBody() const { return m_RigidBody; }
    };
} // namespace MirageEngine::Core::Physics
