#pragma once

#include <iostream>
#include "common_enum.h"
#include <Components/component.h>
#include <Components/transform.h>

namespace MirageEngine
{
    namespace Core
    {
        namespace Physics
        {
            class Bullet3Collider;

            class ICollider : public MirageEngine::Core::Components::Component
            {
                protected:

                    Components::Transform m_Transform;    // collision shape transform (position, rotation, scale)
                    float                 m_Radius = 1.f; // for sphere and capsule
                    float                 m_Height = 2.f; // for capsule
                public:

                    ICollider()          = default;
                    virtual ~ICollider() = default;

                    virtual bool Create(ECollisionShapeType shapeType) = 0;

                    virtual void Init()   = 0;
                    virtual void Update() = 0;

                    Components::Transform& GetTransform() { return m_Transform; }

                    virtual Bullet3Collider& API_Bullet3() { throw std::runtime_error("Bad API Call : object is not a Bullet3Collider"); }
            };
        } // namespace Physics
    } // namespace Core
} // namespace MirageEngine
