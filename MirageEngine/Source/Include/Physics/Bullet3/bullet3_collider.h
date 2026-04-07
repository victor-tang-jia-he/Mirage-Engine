#pragma once

#include "bullet3_pch.h"
#include "../i_collider.h"

namespace MirageEngine
{
    namespace Core
    {
        namespace Physics
        {
            class Bullet3Collider : public ICollider
            {
                private:

                    btCollisionShape* m_collisionShape = nullptr;

                public:

                    Bullet3Collider() = default;
                    ~Bullet3Collider() override;

                    bool Create(ECollisionShapeType shapeType) override;

                    void Init() override {};
                    void Update() override {};

                    btCollisionShape* GetBullet3CollisionShape() const { return m_collisionShape; }

                    Bullet3Collider& API_Bullet3() override { return *this; }
            };
        } // namespace Physics
    } // namespace Core
} // namespace MirageEngine
