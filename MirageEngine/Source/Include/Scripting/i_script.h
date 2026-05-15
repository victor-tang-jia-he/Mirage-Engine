#pragma once

#include <Components/component.h>

namespace MirageEngine::Core::Scripting
{
    class IScript : public Components::Component
    {
        public:

            virtual ~IScript() = default;

            virtual void OnStart() override {};
            virtual void OnUpdate(float dt) override {};
            virtual void OnFixedUpdate(float dt) override {};
            virtual void OnDestroy() override {};

            virtual const char* GetName() const = 0;

            virtual entt::id_type GetType() const override { return 1; };
    };
} // namespace MirageEngine::Core::Scripting
