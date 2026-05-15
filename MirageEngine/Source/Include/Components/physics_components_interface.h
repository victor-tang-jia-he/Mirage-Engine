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

                virtual std::shared_ptr<Physics::ICollider> GetCollision() const = 0;
        };
    } // namespace Components
} // namespace MirageEngine::Core
