#pragma once

namespace MirageEngine::Core::Physics
{
    enum class ECollisionShapeType
    {
        Box,
        Sphere,
        Capsule,
        Mesh
    };

    enum class ECollisionGroup : int
    {
        None = 0,
        Default = 1 << 0,
        Group1 = 1 << 1,
        Group2 = 1 << 2,
        Group3 = 1 << 3,
        Group4 = 1 << 4,
        Group5 = 1 << 5,
        Group6 = 1 << 6,
        Group7 = 1 << 7,
        All = Default | Group1 | Group2 | Group3 | Group4 | Group5 | Group6 | Group7
    };
    constexpr int operator+(ECollisionGroup g) { return static_cast<int>(g); }
    constexpr ECollisionGroup operator|(ECollisionGroup a, ECollisionGroup b)
    {
        return static_cast<ECollisionGroup>(static_cast<int>(a) | static_cast<int>(b));
    }
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
} // namespace MirageEngine::Core::Physics
