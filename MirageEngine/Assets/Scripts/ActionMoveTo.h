#pragma once
#include <AI/ai_action.h>
#include <vector3.h>

namespace MirageEngine::AI
{
    class ActionMoveTo : public AIAction
    {
        private:

            Math::Vector3 m_Target;
            float m_ArrivalDistance = 0.5f;

        public:
            const char* GetName() const override { return "Move To Target"; }
            entt::id_type GetType() const override { return entt::type_id<ActionMoveTo>().hash(); }

            float Evaluate() override
            {
                return 0.5f;
            }

            void Execute(float dt) override
            {
                auto nav = GetOwner()->GetComponent<NavAgent>();
                if (nav)
                {
                    nav->SetDestination(m_TargetPos);
                }
            };

            void SetTarget(Math::Vector3 _pos) { m_Target = _pos; }

            Math::Vector3 GetTarget() const { return m_Target; }
    };
} // namespace MirageEngine::AI
