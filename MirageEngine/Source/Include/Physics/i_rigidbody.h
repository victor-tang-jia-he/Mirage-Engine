#pragma once

#include <iostream>
#include "common_enum.h"
#include <Components/component.h>
#include <GameObject/game_object.h>
#include <vector3.h>

namespace MirageEngine
{
    namespace Core
    {
        namespace Physics
        {
            class Bullet3RigidBody;
            class ICollider;

            class IRigidBody : public MirageEngine::Core::Components::Component
            {

                    using Vector3 = MirageEngine::Core::Math::Vector3;

                protected:

                    float   m_Mass     = 1.f;
                    Vector3 m_Velocity = Vector3(0.f);

                public:

                    IRigidBody()          = default;
                    virtual ~IRigidBody() = default;

                    virtual bool Create(ICollider* shape) = 0;

                    virtual Bullet3RigidBody& API_Bullet3() { throw std::runtime_error("Bad API Call : object is not a Bullet3RigidBody"); }
            };
        } // namespace Physics
    } // namespace Core
} // namespace MirageEngine
