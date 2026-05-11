#pragma once
#include "Scripting/i_script.h"
#include "Components/utility_agent.h"

namespace MirageEngine
{
    namespace Core::Components
    {
        class UtilityAgent;
    }

    namespace AI
    {
        class AIAction : public MirageEngine::Core::Scripting::IScript
        {
            protected:
                Core::Components::UtilityAgent* m_AgentExecutor = nullptr;
                std::string m_ActionName = "BaseAction";
                Core::Components::UtilityAgent* m_OwnerAgent = nullptr;

            public:
               virtual entt::id_type GetType() const override = 0; 

                void OnStart() override { m_AgentExecutor = GetOwner()->GetComponent<Core::Components::UtilityAgent>().get(); }
                void OnUpdate(float dt) override {}

                virtual ~AIAction() = default;

                // returns a score between 0 and 1 (Used in Decide())
                virtual float Evaluate() = 0;
                // Called every frame while the action is active
                virtual void Execute(float _dt) = 0;

                const char* GetName() const override { return m_ActionName.c_str(); }
                void SetActionName(std::string _name) { m_ActionName = _name; }
        };
    } // namespace AI
} // namespace MirageEngine
