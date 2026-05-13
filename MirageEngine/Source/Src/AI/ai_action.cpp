#include "AI/ai_action.h"
#include "GameObject/game_object.h"
#include "Components/utility_agent.h"

namespace MirageEngine::AI
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
} // namespace MirageEngine::AI
