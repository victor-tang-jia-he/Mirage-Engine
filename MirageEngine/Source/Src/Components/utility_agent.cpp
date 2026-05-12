#include <cstdlib>
#include <vector3.h>
#include <string>
#include "Components/utility_agent.h"
#include "GameObject/game_object.h"
#include "application.h"
#include "time_manager.h"
#include "AI/ai_action.h"
#include <entt/entt.hpp>

using namespace MirageEngine::Core::Math;

namespace MirageEngine::Core::Components
{
	UtilityAgent::~UtilityAgent()
	{
		for (AIAction* action : m_actions) {
			delete action;
		}
		m_actions.clear();
	}

	void UtilityAgent::OnStart()
	{
		srand(static_cast <unsigned> (time(0)));

		if (m_Grid == nullptr)
		{
			Application* app = Application::Instance();
			if (app)
			{
				m_Grid = app->GetSceneGraph()->FindComponentInScene<InfluenceGrid>();
			}
		}

		m_DecisionTimer = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / m_DecisionInterval));
	}

	void UtilityAgent::OnUpdate(float _dt)
	{
		m_DecisionTimer -= _dt;
		if (m_DecisionTimer <= 0.0f)
		{
			Decide();
			m_DecisionTimer = m_DecisionInterval;
		}
		if (m_CurrentAction != nullptr)
			m_CurrentAction->Execute(TimeManager::GetDeltaTime());
	}

	void UtilityAgent::AttachMeta(std::shared_ptr<GameObject> _Owner)
	{
		std::shared_ptr<UtilityAgent> compPtr = std::make_shared<UtilityAgent>(*this);
		_Owner->AddComponent<UtilityAgent>(compPtr);
	}

	void MirageEngine::Core::Components::UtilityAgent::AttachNew(std::shared_ptr<GameObject> _owner)
	{
		std::shared_ptr<UtilityAgent> compPtr = std::make_shared<UtilityAgent>(_owner.get());
		_owner->AddComponent<UtilityAgent>(compPtr);
	}

	entt::id_type UtilityAgent::GetType() const
	{
		return entt::type_id<UtilityAgent>().hash();
	}

	void UtilityAgent::Decide()
	{
		if (m_Grid)
		{
			Vector3 pos = GetOwner()->GetPosition();
			int cellX, cellY;
			m_Grid->WorldToGrid(pos, cellX, cellY);

			if (m_Grid->IsInBounds(cellX, cellY))
			{
				int currentIndex = m_Grid->GetIndex(cellX, cellY);
				const std::vector<InfluenceCell>& cells = m_Grid->GetCells();

				size_t numChannels = m_Grid->GetChannelsNames().size();
				m_CurrentCellIntensity.resize(numChannels);
				for (int i = 0; i < numChannels; i++)
				{
					m_CurrentCellIntensity[i] = cells[currentIndex].channels[i];
				}

				AIAction* bestAction = nullptr;
				float highestScore = -1.0f;
				m_LastScores.clear();
				for (AIAction* action : m_actions)
				{
					float score = action->Evaluate();
					if (action == m_CurrentAction)
						score += m_Inertia;
					m_LastScores.push_back(score);

					if (score > highestScore)
					{
						highestScore = score;
						bestAction = action;
					}
				}
				
				if (bestAction != m_CurrentAction)
				{
					if (m_CurrentAction)
						m_CurrentAction->OnStop();
					m_CurrentAction = bestAction;
					if (m_CurrentAction)
						m_CurrentAction->OnStart();
					m_LastDecisionReason = m_CurrentAction ? m_CurrentAction->GetName() + "with score" + std::to_string(highestScore) : "None";
				}
			}
		}
	}

	void UtilityAgent::ModifyInternalStat(const std::string& _name, float _addValue)
	{
		if (m_InternalStats.find(_name) != m_InternalStats.end())
			m_InternalStats[_name] += _addValue;
	}

	float UtilityAgent::GetIntensity(int _channelIndex) const {
		if (_channelIndex >= 0 && _channelIndex < m_CurrentCellIntensity.size())
			return m_CurrentCellIntensity[_channelIndex];
		return 0.0f;
	}

	float UtilityAgent::GetIntensityByName(const std::string& _name) const
	{
		if (!m_Grid) return 0.0f;
		int index = m_Grid->GetChannelsIndex(_name);
		return GetIntensity(index);
	}

	float UtilityAgent::GetInternalStat(const std::string& _name)
	{
		if (m_InternalStats.find(_name) != m_InternalStats.end())
			return m_InternalStats[_name];
		return 0.0f;
	}
}