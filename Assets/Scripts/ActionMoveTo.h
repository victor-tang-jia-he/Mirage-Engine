#pragma once
#include <Scripting/script_registry.h>
#include <AI/ai_action.h>
#include <vector3.h>

namespace MirageEngine::AI
{
    class NavAgent;

    class ActionMoveTo : public MirageEngine::AI::AIAction
    {
        private:

            MirageEngine::Core::Math::Vector3 m_Target;
            float m_ArrivalDistance = 0.5f;

        public:

            ActionMoveTo();

            void RegisteMeta(entt::locator<entt::meta_ctx>::node_type handle) override
            {
                entt::locator<entt::meta_ctx>::reset(handle);
                entt::meta_factory<ActionMoveTo>()
                    .type(entt::type_id<ActionMoveTo>().hash(), "ActionMoveTo")
                    .base<MirageEngine::AI::AIAction>()
                    .data<&ActionMoveTo::SetTarget, &ActionMoveTo::GetTarget>("Target Position")
                    .data<&ActionMoveTo::SetArrivalDistance, &ActionMoveTo::GetArrivalDistance>("Arrival Distance")
                    .ctor<>();
            }

            void OnUpdate(float dt) override {}

            const char* GetName() const override { return m_ActionName.c_str(); }

            entt::id_type GetType() const override;

            float Evaluate() override;

            void Execute(float dt) override;

            void SetTarget(MirageEngine::Core::Math::Vector3 _pos) { m_Target = _pos; }
			void SetArrivalDistance(float _distance) { m_ArrivalDistance = _distance; }

            MirageEngine::Core::Math::Vector3 GetTarget() const { return m_Target; }
			float GetArrivalDistance() const { return m_ArrivalDistance; }
    };
} // namespace MirageEngine::AI
