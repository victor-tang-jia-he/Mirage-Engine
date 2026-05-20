#include "ActionMoveTo.h"
#include "GameObject/game_object.h"
#include "Components/nav_agent.h"
#include <entt/entt.hpp>

namespace MirageEngine::AI
{
    REGISTER_SCRIPT(ActionMoveTo);
    ActionMoveTo::ActionMoveTo()
    {
        m_ActionName = "MoveTo";
        m_FullName = typeid(*this).name();
    }

    float ActionMoveTo::Evaluate()
    {
        return 0.5f; 
    }

    void ActionMoveTo::Execute(float dt)
    {
        auto nav = GetOwner()->GetComponent<MirageEngine::Core::Components::NavAgent>();
        if (nav)
        {
            nav->SetDestination(m_Target);
        }
    }

    entt::id_type ActionMoveTo::GetType() const { return entt::type_id<ActionMoveTo>().hash(); }
} // namespace MirageEngine::AI
