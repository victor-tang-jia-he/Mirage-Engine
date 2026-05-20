#pragma once
#include <Scripting/script_registry.h>
#include <AI/ai_action.h>

namespace MirageEngine::AI {
    class ActionExtinguish : public  MirageEngine::AI::AIAction
    {
    public:
        ActionExtinguish();

        void RegisteMeta(entt::locator<entt::meta_ctx>::node_type handle) override
        {
            entt::meta_factory<ActionExtinguish>()
                .type(entt::type_id<ActionExtinguish>().hash(), "ActionExtinguish")
                .base < MirageEngine::AI::AIAction>()
                .base<MirageEngine::Core::Scripting::IScript>()
                .base<MirageEngine::Core::Components::Component>()
                .ctor<>();
        }

        float Evaluate() override;
        void OnStart() override;
        void OnUpdate(float dt) override {}
        const char* GetName() const override { return m_ActionName.c_str(); }
        entt::id_type GetType() const override;

        void Execute(float dt) override;
    };
}