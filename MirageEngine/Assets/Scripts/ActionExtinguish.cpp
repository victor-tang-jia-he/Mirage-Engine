#include "ActionExtinguish.h"
#include "GameObject/game_object.h"
#include "Components/influence_emitter.h"
#include "Components/utility_agent.h"
#include <MirageEngine/application.h>

namespace MirageEngine::AI {
    REGISTER_SCRIPT(ActionExtinguish);
    ActionExtinguish::ActionExtinguish() { m_ActionName = "ActionExtinguish"; }

    float ActionExtinguish::Evaluate()
    {
        return AIAction::Evaluate();
    }

    entt::id_type ActionExtinguish::GetType() const { return entt::type_id<ActionExtinguish>().hash(); }

    void ActionExtinguish::Execute(float dt) {
        auto nav = GetOwner()->GetComponent<NavAgent>();
        if (nav) nav->GetCurrentPath().clear();

        auto app = Application::Instance();
        auto children = GetOwner()->GetParent()->GetChildren();

        for (auto& child : children)
        {
            if (child->GetName() == "Feu")
            {
                float dist = GetOwner()->GetPosition().Distance(child->GetPosition());
                if (dist < 3.0f)
                {
                    std::cout << "[IA] FEU ETEINT ! Suppression de l'objet." << std::endl;
                    app->GetSceneGraph()->RemoveGameObject(child);
                    break;
                }
            }
        }
    }
}