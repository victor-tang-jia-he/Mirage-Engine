#include "ActionExtinguish.h"
#include "GameObject/game_object.h"
#include "Components/influence_emitter.h"
#include "Components/utility_agent.h"
#include "Components/utility_agent.h"

namespace MirageEngine::AI {
    REGISTER_SCRIPT(ActionExtinguish);
    ActionExtinguish::ActionExtinguish() { m_ActionName = "ActionExtinguish"; }

    float ActionExtinguish::Evaluate() 
    {
        return AIAction::Evaluate();
    }

    entt::id_type ActionExtinguish::GetType() const { return entt::type_id<ActionExtinguish>().hash(); }

    void ActionExtinguish::Execute(float dt) {
        if (!GetOwner() || !GetOwner()->GetParent()) return;

        auto siblings = GetOwner()->GetParent()->GetChildren();

        for (auto& obj : siblings)
        {
            if (obj && obj->GetName() == "Feu")
            {
                float dist = GetOwner()->GetPosition().Distance(obj->GetPosition());
                if (dist < 3.0f)
                {
                    obj->Destroy();
                    std::cout << "[SCRIPT] Extinguishing fire: " << obj->GetName() << std::endl;
                    break;
                }
            }
        }
    }
}