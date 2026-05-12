#pragma once

#include <iostream>
#include <vector3.h>

namespace MirageEngine
{
    namespace Core
    {
        namespace Physics
        {
            enum class ECollisionGroup;
            class Bullet3WorldPhysics;
            class IRigidBody;

            class IWorldPhysics
            {
                    using Vector3 = MirageEngine::Core::Math::Vector3;

                public:

                    IWorldPhysics() = default;
                    virtual ~IWorldPhysics() = default;

                    virtual void AddRigidBody(std::shared_ptr<IRigidBody> _body, ECollisionGroup _group, ECollisionGroup _mask) = 0;
                    virtual void RemoveRigidBody(IRigidBody* _body) = 0;
                    virtual void Update(float _deltaTime) = 0;

                    virtual void SetGravity(float _x, float _y, float _z) = 0;
                    virtual void SetGravity(Vector3& _v) = 0;

                    virtual Bullet3WorldPhysics& API_Bullet3() { throw std::runtime_error("Bad API Call : object is not a Bullet3WorldPhysics"); }
            };
        } // namespace Physics
    } // namespace Core
} // namespace MirageEngine
