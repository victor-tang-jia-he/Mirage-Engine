#pragma once
#include "bullet3_pch.h"
#include "../i_rigidbody.h"

namespace MirageEngine
{
    namespace Core
    {
        namespace Physics
        {
            class Bullet3RigidBody : public IRigidBody
            {
                private:
                    btRigidBody* m_rigidBody = nullptr;

                public:

                    Bullet3RigidBody() = default;
                    ~Bullet3RigidBody() override;

                    bool Create(ICollider* _shape, ERigidBodyType _type) override;

                    void Init()   override {};
                    void Update() override;

                    void OnTransformChanged(MirageEngine::Core::Components::Transform* _newTransform) override;

                    void SetType(ERigidBodyType _type) override;

                    void SetMass(float mass) override;

                    btRigidBody* GetBullet3RigidBody() const { return m_rigidBody; }

                    Bullet3RigidBody& API_Bullet3() override { return *this; }
            };
        } // namespace Physics
    } // namespace Core
} // namespace MirageEngine
