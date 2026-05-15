#pragma once
#include <AI/ai_action.h>
#include <vector3.h>

namespace MirageEngine::AI
{
    class NavAgent; 

    class ActionMoveTo : public AIAction
    {
        private:
            MirageEngine::Core::Math::Vector3 m_Target;
            float m_ArrivalDistance = 0.5f;

        public:
            ActionMoveTo(); 

            const char* GetName() const override { return "Move To Target"; }

            entt::id_type GetType() const override;

            float Evaluate() override;

            void Execute(float dt) override;

            void SetTarget(Vector3 _pos) { m_Target = _pos; }
            MirageEngine::Core::Math::Vector3 GetTarget() const { return m_Target; }
    };
} // namespace MirageEngine::AI
