#pragma once

namespace MirageEngine::Core::Physics
{
    enum class ECollisionShapeType
    {
        BOX,
        SPHERE,
        CAPSULE,
        MESH
    };

    enum class ECollisionGroup : int
    {
        NONE = 0,
        DEFAULT = 1 << 0,
        GROUP_1 = 1 << 1,
        GROUP_2 = 1 << 2,
        GROUP_3 = 1 << 3,
        GROUP_4 = 1 << 4,
        GROUP_5 = 1 << 5,
        GROUP_6 = 1 << 6,
        GROUP_7 = 1 << 7,
        ALL = DEFAULT | GROUP_1 | GROUP_2 | GROUP_3 | GROUP_4 | GROUP_5 | GROUP_6 | GROUP_7
    };

    constexpr int operator+(ECollisionGroup g)
    {
        return static_cast<int>(g);
    }

    constexpr ECollisionGroup operator|(ECollisionGroup a, ECollisionGroup b)
    {
        return static_cast<ECollisionGroup>(static_cast<int>(a) | static_cast<int>(b));
    }
    
    enum class ECollisionResponse
    {
        IGNORE_COLLISION,
        OVERLAP,
        BLOCK
    };

    enum class ERigidBodyType
    {
        STATIC,
        DYNAMIC,
        KINEMATIC
    };

    enum class EPhysicsEngine
    {
        BULLET_3
    };
} // namespace MirageEngine::Core::Physics
