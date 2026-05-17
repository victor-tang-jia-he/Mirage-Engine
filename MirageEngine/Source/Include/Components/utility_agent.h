#pragma once 
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include "Components/component.h" 
#include "AI/influence_grid.h"

namespace MirageEngine::AI
{
	class AIAction;
}
	
namespace MirageEngine::Core::Components
{
	using namespace MirageEngine::AI;

	struct StatDefinition {
		std::string name;
		float initialValue;
	};

	class UtilityAgent : public Component, public std::enable_shared_from_this<UtilityAgent>
	{
	private:
		bool m_StatsInitialized = false;

		/* Configs */
		// DecisionInterval determines how often the agent re-evaluates it's actions
		float m_DecisionInterval = 0.3f;
		// Inertia determines how much the agent sticks to its current action
		float m_Inertia = 0.1f;
		// List of actions the agent can choose from
		std::vector<AI::AIAction*> m_actions;

		/* Perception*/
		InfluenceGrid* m_Grid = nullptr;
		std::vector<float> m_CurrentCellIntensity;

		/* Internal state */
		AI::AIAction* m_CurrentAction = nullptr;
		// Timer to track when to make the next decision
		float m_DecisionTimer = 0.0f;
		// Memory of information like stress/stamina/health
		std::unordered_map<std::string, float> m_InternalStats;
		std::vector<StatDefinition> m_DefaultStats;

		/* Feedback */
		// Stores the reason for the last decision
		std::string m_LastDecisionReason = "None";
		// Stores the score of each action in the last evaluation, for debugging purposes
		std::vector<float> m_LastScores;


	public:
		UtilityAgent() = default;
		UtilityAgent(GameObject* _owner) {}
		virtual ~UtilityAgent();

		void AttachMeta(std::shared_ptr<GameObject> _Owner);
		static void AttachNew(std::shared_ptr<GameObject> _owner);

		entt::id_type GetType() const override;
		
		void OnStart() override;
		void OnUpdate(float _dt) override;

		void AddAction(AI::AIAction* _action) { m_actions.push_back(_action); }
		void Decide();

		void ModifyInternalStat(const std::string& _name, float _addValue);

		// Getters
		float GetDecisionInterval() const { return m_DecisionInterval; }
		float GetInertia() const { return m_Inertia; }
		std::vector<AI::AIAction*> GetActions() { return m_actions; }
        std::vector<AI::AIAction*>& GetActionsRef() { return m_actions; }
		InfluenceGrid* GetInfluenceGrid() const { return m_Grid; }
		float GetIntensity(int _channelIndex) const;
		float GetIntensityByName(const std::string& _name) const;
		float GetInternalStat(const std::string& _name);
		const std::string& GetLastDecisionReason() const { return m_LastDecisionReason; }
        const std::vector<float>& GetLastScores() const { return m_LastScores; }
        const std::vector<float>& GetSensors() const { return m_CurrentCellIntensity; }
        const std::unordered_map<std::string, float>& GetInternalStatsMap() const { return m_InternalStats; }
        InfluenceGrid* GetTargetGrid() { return m_Grid; }

		// Setters
		void SetDecisionInterval(float _newDecisionInterval) { m_DecisionInterval = _newDecisionInterval; }
		void SetInertia(float _newInertia) { m_Inertia = _newInertia; }	
		void SetInfluenceGrid(InfluenceGrid* _newGrid) { m_Grid = _newGrid; }
		void SetInternalStat(const std::string& _name, float _value) { m_InternalStats[_name] = _value; }
		void SetTargetGrid(InfluenceGrid* _grid) { m_Grid = _grid; }
		void SetActions(const std::vector<AI::AIAction*>& _newActions);
	};
}