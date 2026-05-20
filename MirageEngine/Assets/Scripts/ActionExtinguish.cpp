#include "ActionExtinguish.h"
#include "GameObject/game_object.h"
#include "Components/influence_emitter.h"
#include "Components/utility_agent.h"
#include "Components/nav_agent.h"  
#include <entt/entt.hpp>

namespace MirageEngine::AI {
    REGISTER_SCRIPT(ActionExtinguish);
    ActionExtinguish::ActionExtinguish() { m_ActionName = "ActionExtinguish"; }

    float ActionExtinguish::Evaluate()
    {
        return AIAction::Evaluate();
    }

    void ActionExtinguish::OnStart()
    {
        auto nav = GetOwner()->GetComponent<MirageEngine::Core::Components::NavAgent>();
        if (nav) {
            nav->GetCurrentPath().clear();
        }
    }

    entt::id_type ActionExtinguish::GetType() const { return entt::type_id<ActionExtinguish>().hash(); }

    void ActionExtinguish::Execute(float dt)
    {
        GameObject* rootSearch = GetOwner();

        while (rootSearch->GetParent() != nullptr) {
            rootSearch = rootSearch->GetParent().get();
        }
        auto allPotentialTargets = rootSearch->GetChildren();

        for (auto& obj : allPotentialTargets)
        {
            if (obj->GetName() == "Feu" && !obj->IsPendingDestroy())
            {
                float dist = GetOwner()->GetPosition().Distance(obj->GetPosition());

                if (dist < 4.5f)
                {
                    std::cout << "[IA] SCRIPT: Fire sphere found in hierarchy! Destroying..." << std::endl;
                    obj->Destroy();
                    break;
                }
            }
        }
    }
}