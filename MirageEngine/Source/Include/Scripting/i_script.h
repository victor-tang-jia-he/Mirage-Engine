#pragma once

#include <Components/component.h>
#include <iostream>

#include <entt/entt.hpp>
#include <entt/meta/meta.hpp>

namespace MirageEngine::Core::Scripting
{
    class IScript : public Components::Component
    {
        public:

            virtual ~IScript() = default;

            virtual void RegisteMeta(entt::locator<entt::meta_ctx>::node_type handle) { std::cout << "Not Override" << std::endl; };
            virtual void OnStart() {};
            virtual void OnUpdate(float dt) {};
            virtual void OnFixedUpdate(float dt) {};
            virtual void OnDestroy() {};

            virtual const char* GetName() const = 0;

            virtual entt::id_type GetType() const override { return 1; };
    };
} // namespace MirageEngine::Core::Scripting
