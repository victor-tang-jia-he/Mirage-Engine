#pragma once
namespace MirageEngine
{
    namespace Core
    {
        namespace Physics
        {
            enum class ECollisionShapeType
            {
                Box,
                Sphere,
                Capsule,
                Mesh
            };

            enum class ECollisionGroup
            {
                Default = 1 << 0,
                Group1  = 1 << 1,
                Group2  = 1 << 2,
                Group3  = 1 << 3,
                Group4  = 1 << 4,
                Group5  = 1 << 5,
                Group6  = 1 << 6,
                Group7  = 1 << 7
            };

            enum class ECollisionResponse
            {
                Ignore,
                Overlap,
                Block
            };

            enum class ERigidBodyType
            {
                Static,
                Dynamic,
                Kinematic
            };

            enum class EPhysicsEngine
            {
                Bullet3
            };
        } // namespace Physics
    } // namespace Core
} // namespace MirageEngine
