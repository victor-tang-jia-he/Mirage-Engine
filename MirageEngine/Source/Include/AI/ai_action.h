#pragma once

#include <Scripting/i_script.h>
#include <AI/responsive_curve.h>

namespace MirageEngine
{   
    namespace Core::Components
    {
        class UtilityAgent;
    }

    enum class VariableSource
    {
        Grid,
        Blackboard
    };

    struct Consideration
    {
        VariableSource source = VariableSource::Grid;
        std::string variableName = "";
        AI::ResponsiveCurve responseCurve;
        float GetScore(Core::Components::UtilityAgent* _agent);
    };

    namespace AI
    {
        class AIAction : public MirageEngine::Core::Scripting::IScript
        {
        protected:
            std::string m_ActionName = "BaseAction";
            std::string m_FullName = "AIAction";
            Core::Components::UtilityAgent* m_OwnerAgent = nullptr;
            std::vector<Consideration> m_Considerations;

        public:
            virtual entt::id_type GetType() const override = 0; 
            
            void OnStart() override;
            void OnUpdate(float dt) override;
            
            AIAction();
            virtual ~AIAction() = default;
            
            // returns a score between 0 and 1 (Used in Decide())
            virtual float Evaluate();
            // Called every frame while the action is active
            virtual void Execute(float _dt) = 0;
            
            const char* GetName() const override { return m_ActionName.c_str(); }
			std::string GetFullName() const { return m_FullName; }
            void SetActionName(std::string _name) { m_ActionName = _name; }

            std::vector<Consideration>& GetConsiderations() { return m_Considerations; }
            void SetConsiderations(const std::vector<Consideration>& _newCons) { m_Considerations = _newCons; }
        };
    } // namespace AI
} // namespace MirageEngine
