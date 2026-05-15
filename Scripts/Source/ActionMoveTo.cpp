#include "ActionMoveTo.h"
#include "GameObject/game_object.h"
#include "Components/nav_agent.h"
#include <entt/entt.hpp>
REGISTER_SCRIPT(ActionMoveTo);

namespace MirageEngine::AI
{

    ActionMoveTo::ActionMoveTo()
    {
        m_ActionName = "MoveTo";
    }

    float ActionMoveTo::Evaluate()
    {
        return 0.5f; 
    }

    void ActionMoveTo::Execute(float dt)
    {
        auto nav = GetOwner()->GetComponent<Core::Components::NavAgent>();
        if (nav)
        {
            nav->SetDestination(m_Target);
        }
    }

    entt::id_type ActionMoveTo::GetType() const { return entt::type_id<ActionMoveTo>().hash(); }
} // namespace MirageEngine::AI
