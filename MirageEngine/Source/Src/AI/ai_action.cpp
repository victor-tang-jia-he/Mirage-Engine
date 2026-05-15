#include "AI/ai_action.h"
#include "GameObject/game_object.h"
#include "Components/utility_agent.h"

namespace MirageEngine
{
    float Consideration::GetScore(Core::Components::UtilityAgent* _agent)
    {
        if (!_agent)
            return 0.0f;

        float input = 0.0f;
        if (source == VariableSource::Grid)
            input = _agent->GetIntensityByName(variableName);
        else
            input = _agent->GetInternalStat(variableName);

        return responseCurve.Evaluate(input);
    }

    namespace AI
    {
        AIAction::AIAction()
            : m_ActionName("BaseAction")
            , m_OwnerAgent(nullptr)
        {
        }

        void AIAction::OnStart()
        {
            auto agentPtr = GetOwner()->GetComponent<Core::Components::UtilityAgent>();
            if (agentPtr)
            {
                m_OwnerAgent = agentPtr.get();
            }
        }

        void AIAction::OnUpdate(float dt) {}

        float AIAction::Evaluate()
        {
            if (m_Considerations.empty())
                return 0.0f;

            float finalScore = 1.0f;

            for (auto& consideration : m_Considerations)
            {
                finalScore *= consideration.GetScore(m_OwnerAgent);
            }

            return finalScore;
        }
    } // namespace AI
} // namespace MirageEngine::AI
