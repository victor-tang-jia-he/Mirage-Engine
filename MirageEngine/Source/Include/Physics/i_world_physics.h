#pragma once

#include <iostream>
#include <vector3.h>

namespace MirageEngine
{
    namespace Core
    {
        namespace Physics
        {
            class Bullet3WorldPhysics;
            class IRigidBody;

            class IWorldPhysics
            {
                    using Vector3 = MirageEngine::Core::Math::Vector3;

                public:

                    IWorldPhysics()          = default;
                    virtual ~IWorldPhysics() = default;

                    virtual void AddRigidBody(IRigidBody* body)    = 0;
                    virtual void RemoveRigidBody(IRigidBody* body) = 0;
                    virtual void Update(float deltaTime)           = 0;

                    virtual void SetGravity(float x, float y, float z) = 0;
                    virtual void SetGravity(Vector3& v)                = 0;

                    virtual Bullet3WorldPhysics& API_Bullet3() { throw std::runtime_error("Bad API Call : object is not a Bullet3WorldPhysics"); }
            };
        } // namespace Physics
    } // namespace Core
} // namespace MirageEngine
