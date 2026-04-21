#pragma once

namespace MirageEngine::Core
{
    namespace Physics
    {
        class ICollider;
    }

    namespace Components
    {
        class ICCollider
        {
            public:

                virtual Physics::ICollider* GetCollision() const { return nullptr; }
        };
    } // namespace Components
} // namespace MirageEngine::Core
