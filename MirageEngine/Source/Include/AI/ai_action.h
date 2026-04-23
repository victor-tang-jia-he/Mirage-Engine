#pragma once
#include <string>

namespace MirageEngine::AI
{
    namespace Core::Components { class UtilityAgent; }

    class AIAction
    {
    protected:
        std::string m_Name;
        UtilityAgent* m_Owner = nullptr;

    public:
        AIAction(const std::string& _name, UtilityAgent* _owner)
            : m_Name(_name), m_Owner(_owner) {
        }

        virtual ~AIAction() = default;

        // returns a score between 0 and 1 (Used in Decide())
        virtual float Evaluate() = 0;
        // Called every frame while the action is active
        virtual void Execute(float _dt) = 0;
        // Called when the action starts, for setup purposes
        virtual void OnStart() {}
        // Called when the action stops, for cleanup purposes
        virtual void OnStop() {}

        // Getters
        const std::string& GetName() const { return m_Name; }
        UtilityAgent* GetOwner() const { return m_Owner; }
    };
}