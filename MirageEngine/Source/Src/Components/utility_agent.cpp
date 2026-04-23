#include <cstdlib>
#include <vector3.h>
#include <string>
#include "Components/utility_agent.h"
#include "GameObject/game_object.h"
#include "application.h"
#include "time_manager.h"
#include "AI/ai_action.h"

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

	void UtilityAgent::Init()
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

	void UtilityAgent::Update()
	{
		m_DecisionTimer -= TimeManager::GetDeltaTime();
		if (m_DecisionTimer <= 0.0f)
		{
			Decide();
			m_DecisionTimer = m_DecisionInterval;
		}
		if (m_CurrentAction != nullptr)
			m_CurrentAction->Execute(TimeManager::GetDeltaTime());
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

				for (int i = 0; i < InfluenceCell::MAX_CHANNELS; i++)
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
}